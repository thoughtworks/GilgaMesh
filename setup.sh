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
else
	echo 'Unknown platform, aborting.'
	exit
fi

## should check if you do not have wget
#fix folder layout

# ******************************* #
# * JLINK Deployer and Debugger * #
# ******************************* #
if [[ $platform == 'osx' ]]; then
	mkdir $DEPLOY_RESOURCES/jlink
	wget --post-data="agree=1&confirm=yes" "https://www.segger.com/jlink-software.html?step=1&file=JLinkMacOSX_512a" -O $DEPLOY_RESOURCES/jlink/JLink_MacOSX_V512a.pkg
	# should check checksum
	open $DEPLOY_RESOURCES/jlink/JLink_MacOSX_V512a.pkg
elif [[ $platform == 'linux' ]]; then	
	echo '****************************************************************************'
	echo 'For linux, manually install segger JLink tools version 9 from www.segger.com'
	echo '****************************************************************************'
fi

# ********************* #
# * GCC ARM Toolchain * #
# ********************* #
mkdir $GCC_ARM_TOOLCHAIN
#should check checksum
#should get the latest gcc-arm-none-eabi
if [[ $platform == 'osx' ]]; then
	wget -O $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1-20160330-mac.tar.bz2 https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q1-update/+download/gcc-arm-none-eabi-5_3-2016q1-20160330-mac.tar.bz2
	tar -C $GCC_ARM_TOOLCHAIN -xjf $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1-20160330-mac.tar.bz2
	mv $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1/* $GCC_ARM_TOOLCHAIN
	rm -r $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1-20160330-mac.tar.bz2 $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1/
elif [[ $platform == 'linux' ]]; then
	wget -O $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2 https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q1-update/+download/gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2
	tar -C $GCC_ARM_TOOLCHAIN -xjf $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2
	mv $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1/* $GCC_ARM_TOOLCHAIN
	rm -r $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2 $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_3-2016q1/	
fi	

# ********************* #
# * Nordic Softdevice * #
# ********************* #
mkdir $SOFTDEVICE_LOCATION
wget --post-data="confirm_terms_and_conditions=Accept" "https://www.nordicsemi.com/eng/nordic/download_resource/53724/9/22863884" -O $SOFTDEVICE_LOCATION/s130_nrf51_2.0.1.zip
unzip $SOFTDEVICE_LOCATION/s130_nrf51_2.0.1.zip -d $SOFTDEVICE_LOCATION
rm $SOFTDEVICE_LOCATION/s130_nrf51_2.0.1.zip

# ************** #
# * Nordic SDK * #
# ************** #
mkdir $SDK
wget -O $SDK/nRF5_SDK_11.0.0_89a8197.zip https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/nRF5_SDK_11.0.0_89a8197.zip
## should check checksum
unzip $SDK/nRF5_SDK_11.0.0_89a8197.zip -d $SDK/nrf_sdk_11_0
rm $SDK/nRF5_SDK_11.0.0_89a8197.zip
git clone https://github.com/I-SYST/EHAL $DEPLOY_RESOURCES/EHAL

# ************************* #
# * cmocka test framework * #
# ************************* #
mkdir $CMOCKA
wget -O $CMOCKA/cmocka-1.0.1.tar.xz https://cmocka.org/files/1.0/cmocka-1.0.1.tar.xz
tar -C $CMOCKA -xvf $CMOCKA/cmocka-1.0.1.tar.xz
mv $CMOCKA/cmocka-1.0.1/* $CMOCKA
mv $CMOCKA/cmocka-1.0.1/.[!.]* $CMOCKA
rmdir $CMOCKA/cmocka-1.0.1
rm $CMOCKA/cmocka-1.0.1.tar.xz
mkdir $CMOCKA/build
cd $CMOCKA/build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../ -DCMAKE_BUILD_TYPE=Debug ..
make
make install
cd -
	echo 'Installing cmocka library under /usr/local/lib'
if [[ $platform == 'osx' ]]; then
	sudo cp `pwd`/_build/tests/libcmocka.0.dylib /usr/local/lib/libcmocka.0.dylib
elif [[ $platform == 'linux' ]]; then
	sudo cp `pwd`/deploy/cmocka/lib/libcmocka.so.0.3.1 /usr/local/lib/libcmocka.so.0
fi
