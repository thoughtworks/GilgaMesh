#!/bin/bash

NUMBER_OF_DEVICES=`expr $(echo -e "ShowEmuList\nexit\n" | jlinkexe | grep 'J-Link\[' | wc -l) - 1`

FRUITY_REPO=$HOME/Projects/IoT/fruitymesh

for i in $(seq 0 $NUMBER_OF_DEVICES); do
    echo $i | jlinkexe $FRUITY_REPO/deploy/single-fruitymesh-softdevice-deploy.jlink
done
