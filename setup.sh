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
	wget --post-data="agree=1&confirm=yes" "https://www.segger.com/jlink-software.html?step=1&file=JLinkMacOSX_502f" -O $DEPLOY_RESOURCES/jlink/JLink_MacOSX_V502f.pkg
	# should check checksum
	open $DEPLOY_RESOURCES/jlink/JLink_MacOSX_V502f.pkg
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
	wget -O $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2 https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2
	tar -C $GCC_ARM_TOOLCHAIN -xjf $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2
	mv $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4/* $GCC_ARM_TOOLCHAIN
	rm -r $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2 $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4/
elif [[ $platform == 'linux' ]]; then
	wget -O $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2 https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2
	tar -C $GCC_ARM_TOOLCHAIN -xjf $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2
	mv $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4/* $GCC_ARM_TOOLCHAIN
	rm -r $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2 $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4/	
fi	

# ********************* #
# * Nordic Softdevice * #
# ********************* #
mkdir $SOFTDEVICE_LOCATION
wget --post-data="confirm_terms_and_conditions=Accept" "https://www.nordicsemi.com/eng/nordic/download_resource/46579/16/4256576" -O $SOFTDEVICE_LOCATION/s130_nrf51_1.0.0.zip
unzip $SOFTDEVICE_LOCATION/s130_nrf51_1.0.0.zip -d $SOFTDEVICE_LOCATION
rm $SOFTDEVICE_LOCATION/s130_nrf51_1.0.0.zip

# ************** #
# * Nordic SDK * #
# ************** #
mkdir $SDK
wget -O $SDK/nRF51_SDK_9.0.0_2e23562.zip https://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v9.x.x/nRF51_SDK_9.0.0_2e23562.zip
## should check checksum
unzip $SDK/nRF51_SDK_9.0.0_2e23562.zip -d $SDK/nrf_sdk_9_0
rm $SDK/nRF51_SDK_9.0.0_2e23562.zip
git clone https://github.com/I-SYST/EHAL $DEPLOY_RESOURCES/EHAL

# ************************ #
# * Nordic SDK 9.0 Patch * #
# ************************ #
cd $SDK/nrf_sdk_9_0 && wget -qO- https://devzone.nordicsemi.com/attachment/a7a813d4112f2f2f4921a8e6a3a60b67 | patch -p1
cd -
BAD_CONFIG_FILE=$SDK/nrf_sdk_9_0/components/drivers_nrf/config/nrf_drv_config.h
cp $DEPLOY_RESOURCES/nrf_drv_config.h.fix $BAD_CONFIG_FILE

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
	sudo cp `pwd`/_build/tests/libcmocka.so.0 /usr/local/lib/libcmocka.so.0
fi

### BELOW DOES NOT YET WORK ###

#? necessary?
#cp deploy/nrf_svc.h.fix $DEPLOY_RESOURCES/sdk/nrf_sdk_9_0/components/softdevice/s130/headers/nrf_svc.h
#wget -O sdk/nRF52_SDK_0.9.2_dbc28c9.zip https://developer.nordicsemi.com/nRF52_SDK/nRF52_SDK_v0.x.x/nRF52_SDK_0.9.2_dbc28c9.zip
#unzip sdk/nRF52_SDK_0.9.2_dbc28c9.zip -d sdk/nrf52_sdk_0_9_1
#ln -s ehal_2015_09_08 nrf/sdk/ehal_latest
#ln -s ../../arm_cmsis_4_3/CMSIS nrf/sdk/ehal_latest/ARM/CMSIS
#files/Makefile.posix nrf/sdk/nrf_sdk_9_0/components/toolchain/gcc/Makefile.posix
