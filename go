#!/bin/bash

set -e

function helptext {
    echo "Usage: ./go <command>"
    echo ""
    echo "Available commands are:"
    echo "    recovery          Flash the device in recovery mode to retrieve votes"
    echo "    cn                Compile node code"
    echo "    d1                Deploy currently compiled code to a device I choose"
    echo "    fleet             Create and deploy a Fleet: All connected devices become Nodes except for 1 Gateway at oldest attached device"
    echo "    nodes             Create Nodes out of all connected devices"
    echo "    pers              Creates Persistors out of all connected devices"
    echo "    gate              Create and deploy a Gateway to oldest attached device"
    echo "    term <tty.file>   Open terminal to specified file (Requires minicom to be installed)"
    echo "    compile           Clean and compile FruityMesh source"
    echo "    debug             Setup jlinkgdbserver and start gdb"
    echo "    size              Show the size of the FruityMesh.hex and SoftDevice files used for flashing"
    echo "    minprog           Show the progression of file size."
}

function term {
    if [ -z "$1" ]
    then
        echo "No tty file supplied."
        exit 1
    fi
    minicom --device $1 --b 38400
}

function compile {
    make clean && make
}

function replace-line-in-config {
    FILE=$2
    CONFIG_VARIABLE=$3
    ENDING_CONFIG_STATE=$4

    # If given line doesn't contain the config variable we want to toggle
    if [[ `sed "$1q;d" $FILE` != *"$CONFIG_VARIABLE"* ]]
    then
        echo -e "ERROR!"
        echo -e "'$CONFIG_VARIABLE' is not on line $1 in $FILE\n"
        echo -e "You are getting this error because we expect the config file at line $1 to have: '$CONFIG_VARIABLE' but it does not.\n"
        echo -e "I know this dependency is terrible. I'm sorry. Help automate it better."
        exit 1
    fi

    perl -pe "s/.*/$ENDING_CONFIG_STATE/ if $. == $1" < $FILE > config/temporary
    mv config/temporary $FILE
}

GATEWAY_LINE=263
GATEWAY_VARIABLE='IS_GATEWAY_DEVICE'
GATEWAY_ON_CONFIG='#define IS_GATEWAY_DEVICE true'
GATEWAY_OFF_CONFIG='#define IS_GATEWAY_DEVICE false'
GATEWAY_CONFIG_FILE=config/Config.h

function toggle-gateway-config {
    if $1; then
        replace-line-in-config $GATEWAY_LINE $GATEWAY_CONFIG_FILE $GATEWAY_VARIABLE "$GATEWAY_ON_CONFIG"
    else
        replace-line-in-config $GATEWAY_LINE $GATEWAY_CONFIG_FILE $GATEWAY_VARIABLE "$GATEWAY_OFF_CONFIG"
    fi
}

LOGGING_LINE=214
LOGGING_ON_CONFIG='#define ENABLE_LOGGING'
LOGGING_VARIABLE='ENABLE_LOGGING'
LOGGING_OFF_CONFIG_ESCAPED='\/\/#define ENABLE_LOGGING'
LOGGING_FILE=config/Config.h

function toggle-logging-config {
    if $1; then
        replace-line-in-config $LOGGING_LINE $LOGGING_FILE $LOGGING_VARIABLE "$LOGGING_ON_CONFIG"
    else
        replace-line-in-config $LOGGING_LINE $LOGGING_FILE $LOGGING_VARIABLE "$LOGGING_OFF_CONFIG_ESCAPED"
    fi
}

TERM_LINE=215
TERM_ON_CONFIG='#define ENABLE_TERMINAL'
TERM_VARIABLE='ENABLE_TERMINAL'
TERM_OFF_CONFIG_ESCAPED='\/\/#define ENABLE_TERMINAL'
TERM_FILE=config/Config.h

function toggle-terminal-config {
    if $1; then
        replace-line-in-config $TERM_LINE $TERM_FILE $TERM_VARIABLE "$TERM_ON_CONFIG"
    else
        replace-line-in-config $TERM_LINE $TERM_FILE $TERM_VARIABLE "$TERM_OFF_CONFIG_ESCAPED"
    fi
}

NFC_LINE=228
NFC_ON_CONFIG='#define ENABLE_NFC'
NFC_VARIABLE='ENABLE_NFC'
NFC_OFF_CONFIG_ESCAPED='\/\/#define ENABLE_NFC'
NFC_FILE=config/Config.h

function toggle-nfc-config {
    if $1; then
        replace-line-in-config $NFC_LINE $NFC_FILE $NFC_VARIABLE "$NFC_ON_CONFIG"
    else
        replace-line-in-config $NFC_LINE $NFC_FILE $NFC_VARIABLE "$NFC_OFF_CONFIG_ESCAPED"
    fi
}


function create-gateway {
    toggle-gateway-config true
    toggle-logging-config true
    toggle-terminal-config true
    toggle-nfc-config false
    compile
    # Oldest connected J-Link device will become a gateway
    echo 0 | $HOME/nrf/tools/jlink $HOME/nrf/projects/fruitymesh/deploy/single-fruitymesh-softdevice-deploy.jlink
}

function create-persistors {
    toggle-gateway-config false
    toggle-logging-config true
    toggle-terminal-config true
    toggle-nfc-config false
    compile
    # Oldest connected J-Link device will become a persistor
    $HOME/nrf/projects/fruitymesh/deploy/deploy-from-1-on.sh
    #echo 0 | $HOME/nrf/tools/jlink $HOME/nrf/projects/fruitymesh/deploy/single-fruitymesh-softdevice-deploy.jlink
}

function compile-node {
    toggle-gateway-config false
    toggle-terminal-config false
    toggle-logging-config false
    toggle-nfc-config true
    compile
}

function deploy-nodes-to-all-local-devices {
    toggle-gateway-config false
    toggle-terminal-config false
    toggle-logging-config false
    toggle-nfc-config true
    compile
    $HOME/nrf/projects/fruitymesh/deploy/deploy-from-1-on.sh
}

function recovery-mode {
    toggle-gateway-config false
    toggle-terminal-config true
    toggle-logging-config true
    toggle-nfc-config false
    compile
    $HOME/nrf/projects/fruitymesh/deploy/deploy-from-1-on.sh
}

function fleet {
    deploy-nodes-to-all-local-devices
    create-gateway
}

function debug {
    if [ -f $(which jlinkgdbserver) ]; then
        jlinkgdbserver -device nrf51822 -if swd -speed 4000 -noir -port 2331 &
    else
        echo 'Please include jlinkgdbserver on your path.'
        exit 1
    fi

    if [ "$(uname)" == "Darwin" ]; then
        $HOME/nrf/sdk/gcc_arm_embedded_4_9_mac/bin/arm-none-eabi-gdb -q
    elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
        $HOME/nrf/sdk/gcc_arm_embedded_4_9_linux/bin/arm-none-eabi-gdb -q
    fi

    killall jlinkgdbserver
}

function size {
    /usr/local/gcc-arm-none-eabi-4_9-2015q2/bin/arm-none-eabi-size _build/FruityMesh.elf
}


function minprog {
    for file in $(ls size_records); do
        echo $file && cat size_records/$file
    done
}

function deploy-to-device-i-choose {
    $HOME/nrf/tools/jlink deploy/single-fruitymesh-softdevice-deploy.jlink
}

case "$1" in
    recovery) recovery-mode
    ;;
    cn) compile-node
    ;;
    d1) deploy-to-device-i-choose
    ;;
    fleet) fleet
    ;;
    nodes) deploy-nodes-to-all-local-devices
    ;;
    pers) create-persistors
    ;;
    gate) create-gateway
    ;;
    term) term "$2"
    ;;
    compile) compile
    ;;
    debug) debug
    ;;
    size) size
    ;;
    minprog) minprog
    ;;
    *) helptext
    ;;
esac
