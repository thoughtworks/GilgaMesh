#!/bin/bash

set -e

DEPLOY_RESOURCES=deploy
SDK=deploy/sdk
SOFTDEVICE_LOCATION=deploy/softdevice/
GCC_ARM_TOOLCHAIN=deploy/gcc-arm-none-eabi
CMOCKA=deploy/cmocka

function reset-workspace {
    rm -fr $DEPLOY_RESOURCES/jlink $SDK $DEPLOY_RESOURCES/softdevice $DEPLOY_RESOURCES/EHAL $GCC_ARM_TOOLCHAIN $CMOCKA
    exit
}

case "$1" in
    reset) reset-workspace
    ;;
esac

#detect platform
platform='unknown'
unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   platform='linux'
elif [[ "$unamestr" == 'Darwin' ]]; then
   platform='osx'
elif [[ "$unamestr" == 'MSYS_NT-10.0' ]]; then
   platform='windows'
else
	echo 'Unknown platform, aborting.'
	exit
fi

# ********************* #
# * GCC ARM Toolchain * #
# ********************* #
GCC_ARM_TAR_FILE_MAC=gcc-arm-none-eabi-5_4-2016q3-20160926-mac.tar.bz2
GCC_ARM_TAR_FILE_LINUX=gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
GCC_ARM_TAR_FILE_WIN=gcc-arm-none-eabi-5_4-2016q3-20160926-win32.zip
mkdir -p $GCC_ARM_TOOLCHAIN
#should check checksum
#should get the latest gcc-arm-none-eabi
if [ ! -d "$GCC_ARM_TOOLCHAIN/lib/gcc/arm-none-eabi/5.4.1" ]; then
    if [[ $platform == 'osx' ]]; then
        wget -O $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_MAC https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/$GCC_ARM_TAR_FILE_MAC
        tar -C $GCC_ARM_TOOLCHAIN -xjf $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_MAC
        mv -f $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_4-2016q3/* $GCC_ARM_TOOLCHAIN
        rm -r $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_MAC $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_4-2016q3/
    elif [[ $platform == 'linux' ]]; then
        wget -O $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_LINUX https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/$GCC_ARM_TAR_FILE_LINUX
        tar -C $GCC_ARM_TOOLCHAIN -xjf $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_LINUX
        mv -f $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_4-2016q3/* $GCC_ARM_TOOLCHAIN
        rm -r $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_LINUX $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_4-2016q3/
    elif [[ $platform == 'windows' ]]; then
        wget -O $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_WIN https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/$GCC_ARM_TAR_FILE_WIN
        unzip $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_WIN -d $GCC_ARM_TOOLCHAIN
        cp -rf $GCC_ARM_TOOLCHAIN/arm-none-eabi/* $GCC_ARM_TOOLCHAIN
        rm -rf $GCC_ARM_TOOLCHAIN/$GCC_ARM_TAR_FILE_WIN $GCC_ARM_TOOLCHAIN/arm-none-eabi
    fi
fi

# ********************* #
# * Nordic Softdevice * #
# ********************* #
NRF51_SOFTDEVICE_VERSION=s130_nrf51_2.0.1
if [ ! -f "$SOFTDEVICE_LOCATION/$SOFTDEVICE_VERSION_softdevice.hex" ]; then
    mkdir -p $SOFTDEVICE_LOCATION
    wget --post-data="confirm_terms_and_conditions=Accept" "https://www.nordicsemi.com/eng/nordic/download_resource/53724/10/9410912" -O $SOFTDEVICE_LOCATION/$NRF51_SOFTDEVICE_VERSION.zip
    unzip $SOFTDEVICE_LOCATION/$NRF51_SOFTDEVICE_VERSION.zip -d $SOFTDEVICE_LOCATION
    rm $SOFTDEVICE_LOCATION/$NRF51_SOFTDEVICE_VERSION.zip
fi

NRF52_SOFTDEVICE_VERSION=s132_nrf52_3.0.0
if [ ! -f "$SOFTDEVICE_LOCATION/$SOFTDEVICE_VERSION_softdevice.hex" ]; then
    mkdir -p $SOFTDEVICE_LOCATION
    wget --post-data="confirm_terms_and_conditions=Accept" "https://www.nordicsemi.com/eng/nordic/download_resource/56261/5/9072884" -O $SOFTDEVICE_LOCATION/$NRF52_SOFTDEVICE_VERSION.zip
    unzip $SOFTDEVICE_LOCATION/$NRF52_SOFTDEVICE_VERSION.zip -d $SOFTDEVICE_LOCATION
    rm $SOFTDEVICE_LOCATION/$NRF52_SOFTDEVICE_VERSION.zip
fi

# ************** #
# * Nordic SDK * #
# ************** #

if [ ! -d "$SDK/nrf_sdk_12_1" ]; then
    mkdir -p $SDK
    wget -O $SDK/nRF5_SDK_12.1.0_0d23e2a.zip https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v12.x.x/nRF5_SDK_12.1.0_0d23e2a.zip
    ## should check checksum
    unzip $SDK/nRF5_SDK_12.1.0_0d23e2a.zip -d $SDK/nrf_sdk_12_1
    rm $SDK/nRF5_SDK_12.1.0_0d23e2a.zip
fi

if [ ! -d "$DEPLOY_RESOURCES/EHAL" ]; then
    git clone https://github.com/I-SYST/EHAL $DEPLOY_RESOURCES/EHAL
else
    cd $DEPLOY_RESOURCES/EHAL
    git pull --rebase
    cd -
fi

# ************************* #
# * cmocka test framework * #
# ************************* #
if [ ! -d "$CMOCKA" ]; then
    mkdir -p $CMOCKA
    wget -O $CMOCKA/cmocka-1.1.0.tar.xz https://cmocka.org/files/1.1/cmocka-1.1.0.tar.xz
    tar -C $CMOCKA -xvf $CMOCKA/cmocka-1.1.0.tar.xz
    mv -f $CMOCKA/cmocka-1.1.0/* $CMOCKA
    mv -f $CMOCKA/cmocka-1.1.0/.[!.]* $CMOCKA
    rmdir $CMOCKA/cmocka-1.1.0
    rm $CMOCKA/cmocka-1.1.0.tar.xz
fi
mkdir -p $CMOCKA/build
cd $CMOCKA/build
if [[ $platform == 'osx' ]] || [[ $platform == 'linux' ]]; then
  cmake -DCMAKE_C_COMPILER=/usr/bin/gcc
fi
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../ -DCMAKE_BUILD_TYPE=Debug ..
make
make install
cd -
	echo 'Installing cmocka library under /usr/local/lib'
if [[ $platform == 'osx' ]]; then
	sudo cp `pwd`/deploy/cmocka/lib/libcmocka.0.dylib /usr/local/lib/libcmocka.0.dylib
elif [[ $platform == 'linux' ]]; then
	sudo cp `pwd`/deploy/cmocka/lib/libcmocka.so.0.4.0 /usr/local/lib/libcmocka.so.0
elif [[ $platform == 'windows' ]]; then
    cp `pwd`/deploy/cmocka/lib/libcmocka.dll.a /usr/lib/
fi

# ******************************* #
# * JLINK Deployer and Debugger * #
# ******************************* #
# should check checksum
JLINK_PKG_MAC=JLink_MacOSX_V610f.pkg
JLINK_DEB_LINUX=jlink_6.0.7_x86_64.deb
if [[ $platform == 'osx' ]]; then
    if [ ! -f "$DEPLOY_RESOURCES/jlink/$JLINK_PKG_MAC" ]; then
	    mkdir -p $DEPLOY_RESOURCES/jlink
	    wget --post-data="accept_license_agreement=accepted&submit=Download&nbspsoftware" "https://www.segger.com/downloads/jlink/$JLINK_PKG_MAC" -O $DEPLOY_RESOURCES/jlink/$JLINK_PKG_MAC
	fi
	if [ ! -f "$DEPLOY_RESOURCES/jlink/$JLINK_PKG_MAC" ]; then
	    echo '**********************************************************************************************************'
        echo 'Manually install JLink tools from https://www.segger.com/downloads/jlink/$JLINK_PKG_MAC'
        echo '**********************************************************************************************************'
    fi
elif [[ $platform == 'linux' ]]; then
	echo '**********************************************************************************************************'
	echo 'For Linux, manually install JLink tools from https://www.segger.com/downloads/jlink/$JLINK_DEB_LINUX'
	echo '**********************************************************************************************************'
fi