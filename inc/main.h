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
#include <app_timer_appsh.h>
#include <app_util.h>
#include <ble.h>
#include <ble_db_discovery.h>
#include <ble_hci.h>
#include <ctype.h>
#include <device_manager.h>
#include <nordic_common.h>
#include <nrf_gpio.h>
#include <nrf_log.h>
#include <nrf_mbr.h>
#include <nrf_sdm.h>
#include <softdevice_handler.h>

#include "boards.h"
#include "util.h"

#define NRF_UICR_BOOT_START_ADDRESS         (NRF_UICR_BASE + 0x14) // Register where the bootloader start address is stored in the UICR register.


#define SCHED_MAX_EVENT_DATA_SIZE       (sizeof(ble_evt_t))                         /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE                10                                          /**< Maximum number of events in the scheduler queue. */


void initialize();
void run();
void panic();

#ifndef TESTING
int main();
#endif
