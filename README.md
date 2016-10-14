#The beginning of a thoughtful mesh...

This is a bluetooth mesh implementation we wrote in C to work for the NRF51 microcontroller. Our plan is to expand this mesh to support different setups.
If you deploy this code to multiple NRF51 chips (we used the Nordic 51822 device), they will construct a self-healing bluetooth mesh.

This mesh is currently very basic. It supports handshake messages (sharing data between devices to form connections), heartbeat messages
(continuously sent to the root of the mesh to track active peripheral devices), and broadcast messages (short text messages sent throughout the mesh).
You can use this library and expand upon it. (maybe by adding new message types or mesh-forming algorithms, dependent on your use case)

Our current mesh algorithm forms a root-reliant mesh, which requires the presence of a "gateway" device to act as the root of the mesh.
Devices with this code will not connect to each other via BLE unless the gateway device is present.

We are using SDK12 for the NRF51/52 chip, version 2.0.1 of the S130 softdevice, and no bootloader.

###Setup
Note: Currently the setup system only works for Mac OS and Linux, but we would like to host a tested setup script for other environments! Please consider contributing to help us do so.

To quickly setup the workspace, run:
```
./setup.sh
```
Make sure you have installed cmake and wget prior to running this setup script. If you have not, and you are on Mac OS, then run:
```
brew install cmake
brew install wget
```

###Go script
This script allows you to easily compile, deploy, run tests and other useful actions.
To see the possible commands, run:
```
./go
```

###Testing
We use CMocka to run our unit tests. If you would like to contribute to this project, please write tests for any features you develop.
Follow the patterns we have used in the test directory for guidance.
