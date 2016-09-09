#pragma once

#define BOOTLOADER_REGION_START 0x1D000         // this needs to match the FLASH ORIGIN address in the linker script

void softdevice_initialize(void);
