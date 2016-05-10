#pragma once

#include <app_error.h>
#include <app_timer.h>
#include <app_util.h>
#include <ble.h>
#include <ble_db_discovery.h>
#include <ble_hci.h>
#include <device_manager.h>
#include <nordic_common.h>
#include <nrf_gpio.h>
#include <nrf_log.h>
#include <nrf_mbr.h>
#include <nrf_sdm.h>
#include <pstorage.h>
#include <softdevice_handler.h>

#include "boards.h"
#include "bsp.h"

#define NRF_UICR_BOOT_START_ADDRESS         (NRF_UICR_BASE + 0x14) // Register where the bootloader start address is stored in the UICR register.
#define BOOTLOADER_REGION_START 0x1B000                            // this needs to match the FLASH ORIGIN address in the linker script

#define CENTRAL_LINK_COUNT      1    // Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT   0    // Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/


uint32_t initialize();
uint32_t run();
int main(void);
