gcc -o tests/pn532.out tests/unit/Pn532Test.c -Isrc -Iinc -Ideploy/sdk/nrf_sdk_9_0/components/libraries/util -Ideploy/sdk/nrf_sdk_9_0/components/device -Itests/mocks/sdk/toolchain
./tests/pn532.out
rm tests/pn532.out
