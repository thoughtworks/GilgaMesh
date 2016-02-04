#!/bin/bash

set -e

DEPLOY_RESOURCES=deploy
GCC_ARM_TOOLCHAIN=deploy/gcc-arm-none-eabi

function reset-workspace {
    rm -r $DEPLOY_RESOURCES/jlink $DEPLOY_RESOURCES/sdk $DEPLOY_RESOURCES/softdevice $GCC_ARM_TOOLCHAIN
    exit
}

case "$1" in
    reset) reset-workspace
    ;;
esac

## should check if you do not have wget
#fix folder layout

# ******************************* #
# * JLINK Deployer and Debugger * #
# ******************************* #
mkdir $DEPLOY_RESOURCES/jlink
wget --post-data="agree=1&confirm=yes" "https://www.segger.com/jlink-software.html?step=1&file=JLinkMacOSX_502f" -O $DEPLOY_RESOURCES/jlink/JLink_MacOSX_V502f.pkg
## should check checksum
open $DEPLOY_RESOURCES/jlink/JLink_MacOSX_V502f.pkg
#rm -r $DEPLOY_RESOURCES/jlink #should remove this when the jlink package is installed

# ********************* #
# * GCC ARM Toolchain * #
# ********************* #
mkdir $GCC_ARM_TOOLCHAIN
## should check checksum
## should get the latest gcc-arm-none-eabi
## should work for linux too
wget -O $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2 https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2
tar -C $GCC_ARM_TOOLCHAIN -xjf $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2
mv $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4/* $GCC_ARM_TOOLCHAIN
rm -r $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4-20151219-mac.tar.bz2 $GCC_ARM_TOOLCHAIN/gcc-arm-none-eabi-5_2-2015q4/

# ********************* #
# * Nordic Softdevice * #
# ********************* #
wget --post-data="confirm_terms_and_conditions=Accept" "https://www.nordicsemi.com/eng/nordic/download_resource/46579/16/4256576" -O deploy/s130_nrf51_1.0.0.zip
mkdir $DEPLOY_RESOURCES/softdevice
unzip $DEPLOY_RESOURCES/s130_nrf51_1.0.0.zip -d deploy/softdevice
rm $DEPLOY_RESOURCES/s130_nrf51_1.0.0.zip

# ************** #
# * Nordic SDK * #
# ************** #
mkdir $DEPLOY_RESOURCES/sdk
wget -O $DEPLOY_RESOURCES/sdk/nRF51_SDK_9.0.0_2e23562.zip https://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v9.x.x/nRF51_SDK_9.0.0_2e23562.zip
## should check checksum
unzip $DEPLOY_RESOURCES/sdk/nRF51_SDK_9.0.0_2e23562.zip -d $DEPLOY_RESOURCES/sdk/nrf_sdk_9_0
rm $DEPLOY_RESOURCES/sdk/nRF51_SDK_9.0.0_2e23562.zip
git clone https://github.com/I-SYST/EHAL $DEPLOY_RESOURCES/sdk/EHAL

### BELOW DOES NOT YET WORK ###

#? necessary?
#wget -O sdk/nRF52_SDK_0.9.2_dbc28c9.zip https://developer.nordicsemi.com/nRF52_SDK/nRF52_SDK_v0.x.x/nRF52_SDK_0.9.2_dbc28c9.zip
#unzip sdk/nRF52_SDK_0.9.2_dbc28c9.zip -d sdk/nrf52_sdk_0_9_1
#ln -s ehal_2015_09_08 nrf/sdk/ehal_latest
#ln -s ../../arm_cmsis_4_3/CMSIS nrf/sdk/ehal_latest/ARM/CMSIS
#files/Makefile.posix nrf/sdk/nrf_sdk_9_0/components/toolchain/gcc/Makefile.posix
#files/Makefile.posix nrf/sdk/nrf52_sdk_0_9_1/components/toolchain/gcc/Makefile.posix

#? sdk patch
#files/nrf_svc.h nrf/sdk/nrf_sdk_9_0/components/softdevice/s130/headers/nrf_svc.h
#cd nrf/sdk/nrf_sdk_9_0 && wget -qO- https://devzone.nordicsemi.com/attachment/a7a813d4112f2f2f4921a8e6a3a60b67 | patch -p1
#sed -i 's/^#define GPIOTE_ENABLED.*/#define GPIOTE_ENABLED 1/' nrf/sdk/nrf_sdk_9_0/components/drivers_nrf/config/nrf_drv_config.h
