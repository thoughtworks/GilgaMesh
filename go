#!/bin/bash

set -e

JLINK=$HOME/nrf/tools/jlink
JLINKGDBSERVER=/usr/local/bin/jlinkgdbserver
GCC_ARM_SIZE=/usr/local/gcc-arm-none-eabi-4_9-2015q2/bin/arm-none-eabi-size
GDB_ARM_MAC=$HOME/nrf/sdk/gcc_arm_embedded_4_9_mac/bin/arm-none-eabi-gdb
GDB_ARM_LINUX=$HOME/nrf/sdk/gcc_arm_embedded_4_9_linux/bin/arm-none-eabi-gdb

function compile {
    make clean && make
}

function compile-deploy {
    compile
    deploy-to-many
}

function deploy-to-many {
    NUMBER_OF_DEVICES=`expr $(echo -e "ShowEmuList\nexit\n" | $JLINK | grep 'J-Link\[' | wc -l) - 1`
    for i in $(seq 0 $NUMBER_OF_DEVICES); do
        echo $i | $JLINK deploy/single-fruitymesh-softdevice-deploy.jlink
    done
}

function deploy-to-device-i-choose {
    $JLINK deploy/single-fruitymesh-softdevice-deploy.jlink
}

function size {
    $GCC_ARM_SIZE _build/FruityMesh.elf
}

function debug {
    if [ -f $(which jlinkgdbserver) ]; then
        $JLINKGDBSERVER -device nrf51822 -if swd -speed 4000 -noir -port 2331 &
    else
        echo 'Please include jlinkgdbserver on your path.'
        exit 1
    fi

    if [ "$(uname)" == "Darwin" ]; then
        $GDB_ARM_MAC -q
    elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
        $GDB_ARM_LINUX -q
    fi

    killall jlinkgdbserver
}

function minprog {
    for file in $(ls size_records); do
        echo $file && cat size_records/$file
    done
}

function sterm {
    if [ -z "$1" ]
    then
        echo "No device file supplied."
        exit 1
    fi
    screen $1 38400
}

function mterm {
    if [ -z "$1" ]
    then
        echo "No device file supplied."
        exit 1
    fi
    minicom --device $1 --b 38400
}

function helptext {
    echo "Usage: ./go <command>"
    echo ""
    echo "Available commands are:"
    echo "    c                 Compile current code"
    echo "    cd                Compile and deploy current code to all connected devices"
    echo "    s <device file>      Open screen terminal to specified device at baudrate 38400 (Requires screen to be installed)"
    echo "    m <device file>      Open minicom terminal to specified device at baudrate 38400 (Requires minicom to be installed)"
    echo ""
    echo "Less often used commands..."
    echo "    d1                Deploy currently compiled code to a device I choose"
    echo "    size              Show the size of the currently compiled firmware"
    echo "    debug             Setup jlinkgdbserver and start gdb"
    echo "    minprog           Show the code minimization progression"
}

case "$1" in
    c) compile
    ;;
    cd) compile-deploy
    ;;
    d1) deploy-to-device-i-choose
    ;;
    s) sterm "$2"
    ;;
    m) mterm "$2"
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
