#pragma once

/* Family selection for family includes. */
#if defined (NRF51)
#include <nrf51.h>
#include <nrf51_bitfields.h>
#include <nrf51_deprecated.h>
#elif defined (NRF52)
#include <nrf52.h>
   #include <nrf52_bitfields.h>
   #include <nrf51_to_nrf52.h>
   #include <nrf52_name_change.h>
#else
   #error "Device family must be defined. See nrf.h."
#endif /* NRF51, NRF52 */

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
#include "util.h"

#define NRF_UICR_BOOT_START_ADDRESS         (NRF_UICR_BASE + 0x14) // Register where the bootloader start address is stored in the UICR register.
#define BOOTLOADER_REGION_START 0x1B000                            // this needs to match the FLASH ORIGIN address in the linker script

#define CENTRAL_LINK_COUNT      3    // Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT   1    // Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define SCHED_MAX_EVENT_DATA_SIZE       MAX(APP_TIMER_SCHED_EVT_SIZE,\
                                            BLE_STACK_HANDLER_SCHED_EVT_SIZE)       /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE                10                                          /**< Maximum number of events in the scheduler queue. */


void initialize();
uint32_t run();
void panic();

#ifndef TESTING
int main();
#endif
