# Gilgamesh
## The beginning of a thoughtful mesh
Gilgamesh is a bluetooth mesh implementation written in C and designed to run on Nordic Semiconductor's nRF51 microcontroller. If you deploy this code to multiple nRF51 chips (we used the Nordic nRF51822 device), they will construct a self-healing bluetooth mesh and have the ability to pass bluetooth messages between each other.

## Mesh Features:

### Communication between devices:
This mesh currently supports "handshake" messages (sharing data between devices to form BLE connections), "heartbeat" messages (continuously sent to the root of the mesh to track active peripheral devices), and "broadcast" messages (short text messages sent throughout the mesh).

### Mesh-forming Algorithm:
Our current mesh algorithm forms a root-reliant mesh, which requires the presence of a "gateway" device to act as the root of the mesh. Devices in the mesh will not connect to each other via BLE unless the gateway device is present.

### Customization:
You can customize this basic mesh by adding new message types or new mesh-forming algorithms that work best for your use case. In terms of hardware, you can also develop your own PCBs to interact with the nRF51. For example, we connected our chip to 6 RGB LEDs, a buzzer, and an NFC reader because this is what our use case required.

### Firmware/Chip Information
Our mesh implementation is based on Nordic's SDK12 for the NRF51/52 chip, version 2.0.1 of the S130 softdevice, and does not require a bootloader. This was tested on the nRF51822 chip, which is the largest in its chip family. You might need to do some fine tuning make it work on chips with smaller RAM and processing power, like the nRF51422, for example.
<br> 
<img height=300 src="https://github.com/IOTWorks/MeshyMesh/blob/opensource/nordic_chip.jpg" />
</br>
## Usage example

Here we will outline the use case that motivated the development of this mesh:

We developed this mesh to link together a set of our custom-made PCBs that run off of Nordic Semiconductor's nRF51822 chip. These "devices" need to be able to communicate about their current state (e.g. what data they have received from external NFC tag reads) between each other. Eventually, this information has to be passed up to a central device (our gateway node) which is a nRF51822 dongle running a server one of our personal machines. This gateway aggregates the information it collects from all the other devices in the mesh and presents it via a web interface.

We wanted our mesh to rebuild itself continuously because it is being used in an environment where people will constantly be moving around and between the devices (water - a.k.a. humans - interrupt BLE signals). We needed all devices to communicate back to a single device because data visualization via a web interface was a requirement. But maybe your requirements are different! Play with the mesh to make it work best for your needs.
<br> 
<img height=300 src="https://github.com/IOTWorks/MeshyMesh/blob/opensource/devices_talking.jpg" />
</br>

## Installation and development setup

Currently the setup system only works for Mac OS and Linux, but we would like to host a tested setup script for other environments! Please consider contributing to help us do so.

Make sure you have installed cmake, wget, and gcc prior to running this setup script. If you have not, and you are on Mac OS, then run:
```
brew install cmake
brew install wget
brew install gcc
```

NOTE: Installing gcc can take a long time (sometimes on the order of hours). It will appear to get stuck at "make bootstrap" step. Don't worry, be patient, it's just working hard in the background!

Then, to quickly setup the workspace, run:
```
./setup.sh
```

## How to develop and deploy
This mesh does not yet support over-the-air updates, but with the go script, updating the firmware on a single device is simple. Just connect to the device via a USB port and deploy via the go script. To incorporate the changes you make to the source code, make sure to deploy the newest version of your code to every device in the mesh

The go script allows you to easily compile and deploy the firmware, run unit tests, and perform other useful actions. To see the possible commands, run:
```sh
./go
```

## Testing
We use CMocka (v.1.1.0) to run our unit tests. If you would like to contribute to this project, please write tests for any features you develop and follow the patterns we have used in the test directory for guidance.

## Release History

* 0.5.0
    * first O.S. release

## Meta

ThoughtWorks Inc. Emerging Technologies Lab (https://www.thoughtworks.com/insights/iot) – ???@thoughtworks.com
