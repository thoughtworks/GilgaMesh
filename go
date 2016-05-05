#!/bin/bash

set -e

JLINK=/usr/local/bin/JLinkExe
JLINKGDBSERVER=/usr/local/bin/jlinkgdbserver
GCC_ARM_TOOLCHAIN=deploy/gcc-arm-none-eabi/
GCC_ARM_SIZE=$GCC_ARM_TOOLCHAIN/bin/arm-none-eabi-size
GDB_ARM_MAC=$GCC_ARM_TOOLCHAIN/bin/arm-none-eabi-gdb
NUM_CPUS=${NUM_CPUS:=1}
BOARD=${BOARD:=PCA10028}

function compile {
    cd _build
    cmake -DCMAKE_BUILD_TYPE=Debug -DIS_TEST_BUILD=False -DIS_PRODUCTION_BOARD=False -DNFC_DEBUG=False ..
    make clean && make
    cd -
}

function compile-deploy-all {
    compile
    deploy-to-many
}

function compile-deploy-one-swd {
    compile
    echo 0 | $JLINK deploy/single-softdevice-meshymesh-deploy-swd.jlink
}

function deploy-to-many {
    NUMBER_OF_DEVICES=`expr $(echo -e "ShowEmuList\nexit\n" | $JLINK | grep 'J-Link\[' | wc -l)`
    for i in $(seq 0 $(($NUMBER_OF_DEVICES-1))); do
        echo $i | $JLINK deploy/single-mesh-only-deploy.jlink
    done
}

function deploy-to-device-i-choose {
    $JLINK deploy/single-mesh-only-deploy.jlink
}

function reset-device {
    $JLINK deploy/reset-to-factory.jlink
}

function compile-bootloader {
    cd bootloader
    make
    cd -
}

function compile-bootloader-seeed {
    cd bootloader
    TARGET_BOARD=BOARD_VOTING_BOX make
    cd -

    if [[ `uname` == 'Darwin' ]]; then
        cp deploy/softdevice/s130_nrf51_1.0.0_softdevice.hex _build/Debug/
        cp bootloader/_build/nrf51422_xxac.hex _build/Debug/
        open _build/Debug/
    fi
}

function reset-and-deploy-bootloader {
    reset-device
    compile-bootloader
    echo 0 | $JLINK deploy/single-softdevice-bootloader-deploy.jlink
}

function archive {
    MAJOR_VERSION=`sed -n 's/#define APP_VERSION_MAIN //p' config/version.h`
    MINOR_VERSION=`sed -n 's/#define APP_VERSION_SUB //p' config/version.h`
    compile
    nrfutil dfu genpkg meshyMesh_dev_v$MAJOR_VERSION.$MINOR_VERSION.zip --application _build/Debug/MeshyMesh.hex
}

function archive-seeed {
    MAJOR_VERSION=`sed -n 's/#define APP_VERSION_MAIN //p' config/version.h`
    MINOR_VERSION=`sed -n 's/#define APP_VERSION_SUB //p' config/version.h`
    compile-seeed
    nrfutil dfu genpkg meshyMesh_prod_v$MAJOR_VERSION.$MINOR_VERSION.zip --application _build/Debug/MeshyMesh.hex
}

function size {
    $GCC_ARM_SIZE _build/Debug/MeshyMesh.elf
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
    echo "Usage: BOARD=TYPE ./go <command>"
    echo ""
    echo "Available commands are:"
    echo "    ut                Run all unit tests"
    echo "    d                 Deploy to all connected nrf51 prototype boards"
    echo "    c                 Compile without deployment"
    echo "    cs                Compile for Seeed production board"
    echo "    cd                Compile and deploy current code to all connected devices"
    echo "    cds               Compile and deploy current code to one device via swd"
    echo "    r                 Resets one device to factory settings"
    echo "    b                 Resets one device and installs the softdevice and bootloader"
    echo "    bs                Compiles the bootloader for Seeed production board"
    echo "    a                 Compile and archive the current code"
    echo "    as                Compile and archive the current code for Seeed production board"
    echo "    s <device file>      Open screen terminal to specified device at baudrate 38400 (Requires screen to be installed)"
    echo "    m <device file>      Open minicom terminal to specified device at baudrate 38400 (Requires minicom to be installed)"
    echo ""
    echo "Less often used commands..."
    echo "    d1                Deploy currently compiled code to a device I choose"
    echo "    size              Show the size of the currently compiled firmware"
    echo "    debug             Setup jlinkgdbserver and start gdb"
    echo "    minprog           Show the code minimization progression"
    echo ""
    echo "Available boards are:"
    echo "    PCA10028(default) PCA10031 PCA10036 BLE400"
}

function run-tests {
    cd _build
    cmake -DCMAKE_BUILD_TYPE=Debug -DIS_TEST_BUILD=True ..
    make
    cd -

    for TEST in `ls _build/tests/*Test`;
    do
        ./$TEST
    done
}

function deploy {
    cd _build
    cmake -DCMAKE_BUILD_TYPE=Debug -DIS_TEST_BUILD=False -DIS_PRODUCTION_BOARD=False -DNFC_DEBUG=False ..
    make
    cd -

    deploy-to-many
}

function deploy-nfc {
    cd _build
    cmake -DCMAKE_BUILD_TYPE=Debug -DIS_TEST_BUILD=False -DIS_PRODUCTION_BOARD=False -DNFC_DEBUG=True ..
    make clean && make
    cd -

    deploy-to-many
}

function compile-seeed {
    cd _build
    cmake -DCMAKE_BUILD_TYPE=Debug -DIS_TEST_BUILD=False -DIS_PRODUCTION_BOARD=True ..
    make clean && make
    cd -

    if [[ `uname` == 'Darwin' ]]; then
        cp deploy/softdevice/s130_nrf51_1.0.0_softdevice.hex _build/Debug/
        open _build/Debug/
    fi
}

case "$1" in
    ut) run-tests
    ;;
    d) deploy
    ;;
    dn) deploy-nfc
    ;;
    c) compile
    ;;
    cs) compile-seeed
    ;;
    cd) compile-deploy-all
    ;;
    cds) compile-deploy-one-swd
    ;;
    d1) deploy-to-device-i-choose
    ;;
    r) reset-device
    ;;
    b) reset-and-deploy-bootloader
    ;;
    bs) compile-bootloader-seeed
    ;;
    a) archive
    ;;
    as) archive-seeed
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
