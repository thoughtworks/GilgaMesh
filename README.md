#The beginning of a thoughtful mesh...

###Setup
Note: Currently the setup system only works for Mac OS but we would like to host a tested setup script for other environments! Please consider contributing to help us do so.

To quickly setup the workspace, run:
```
./setup.sh
```

###Shortcuts
There are a few useful shortcuts made available in the .shortcuts file. If you would like to use them, run:
```
source .shortcuts
```

###Go script
There are some environment variables to control the go script:
- DFU_DEBUG_FLAGS
enable this flag to flash dfu before enter the bootloader and start dfu  
export `DFU_DEBUG_FLAGS=-DDFU_DEBUG` before runs the go script
- NUM_CPUS
enable parallel compiling, usually set this variable to the number of your cpu cores  
e.g. export `NUM_CPUS=8` before runs the go script  
because the makefile does't handle the dependency perfectly, you may get a compiling error sometimes. just rerun the go script or make command
- BOARD
select the boards definition, the board type is defined in the `boards.h`  
e.g. export `BOARD=BLE400`, the default value is `PCA10028`  
currently the supported values are: `PCA10028` `PCA10031` `PCA10036` `BLE400`

for example, if you have 4 cpu cores and want to enable dfu test on BLE400, you can run:
`DFU_DEBUG_FLAGS=-DDFU_DEBUG BOARD=BLE400 NUM_CPUS=4 ./go c`
