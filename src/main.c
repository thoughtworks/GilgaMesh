#include "main.h"

#include <app_scheduler.h>
#include <softdevice_handler_appsh.h>
#include <connection.h>
#include <gap.h>
#include "error.h"
#include "terminal.h"
#include "app/led.h"

#include "system/storage.h"

#define APP_TIMER_PRESCALER         0                                  /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE     4                                  /**< Size of timer operation queues. */

static uint16_t sizeOfCurrentEvent = sizeof(currentEventBuffer);

// This variable ensures that the linker script will write the bootloader start
// address to the UICR register. This value will be written in the HEX file and
// thus written to UICR when MeshyMesh is flashed into the chip. */

#ifndef TESTING
volatile uint32_t m_uicr_bootloader_start_address  __attribute__ ((section(".uicrBootStartAddress"))) = BOOTLOADER_REGION_START;
#endif

void panic()
{
  NRF_LOG_PRINTF("PANIC!!!\r\n");
  for(;;) { }
}


void softdevice_fault_callback(uint32_t id, uint32_t pc, uint32_t info)
{
  app_error_fault_handler(id, pc, info); // delegate to error module
}

static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
  handle_gap_event(p_ble_evt); // delegate to gap module
}

static void sys_evt_dispatch(uint32_t sys_evt)
{
  sys_handle_storage_event(sys_evt); // delegate to pstorage
}

void softdevice_initialize() {
  sd_mbr_command_t com = {SD_MBR_COMMAND_INIT_SD, };
  nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

  // Initialize SoftDevice.
  NRF_LOG_PRINTF("  * mbr initialization\r\n");
  EC(sd_mbr_command(&com));

  NRF_LOG_PRINTF("  * vector table\r\n");
  EC(sd_softdevice_vector_table_base_set(BOOTLOADER_REGION_START));

  // Initialize the SoftDevice handler module.
  NRF_LOG_PRINTF("  * softdevice handler\r\n");
  SOFTDEVICE_HANDLER_APPSH_INIT(&clock_lf_cfg, true);

  ble_enable_params_t ble_enable_params;
  NRF_LOG_PRINTF("  * BLE params: maximum of %d connections as central and %d connections as peripheral\r\n", CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);
  EC(softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                          PERIPHERAL_LINK_COUNT,
                                          &ble_enable_params));

  // Enable BLE stack.
  NRF_LOG_PRINTF("  * enable softdevice\r\n");
  EC(softdevice_enable(&ble_enable_params));

  // Register with the SoftDevice handler module for BLE events.
  NRF_LOG_PRINTF("  * register BLE event handler\r\n");
  EC(softdevice_ble_evt_handler_set(ble_evt_dispatch));

  // Register with the SoftDevice handler module for system events.
  NRF_LOG_PRINTF("  * register system event handler\r\n");
  EC(softdevice_sys_evt_handler_set(sys_evt_dispatch));
}

void initialize() {
  NRF_LOG_INIT();
  NRF_LOG_PRINTF("\r\n[[ MeshyMesh is booting ]]\r\n");


  NRF_LOG_PRINTF("Init scheduler... \r\n");
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

  NRF_LOG_PRINTF("Init timers... \r\n");
  APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);

  NRF_LOG_PRINTF("Init softdevice... \r\n");
  softdevice_initialize();

  NRF_LOG_PRINTF("Init terminal... \r\n");
  terminal_initialize();

  NRF_LOG_PRINTF("Init command... \r\n");
  command_initialize();

  NRF_LOG_PRINTF("Init leds... \r\n");
  led_initialize();

  NRF_LOG_PRINTF("Init real time clock... \r\n");
  rtc_init();

  NRF_LOG_PRINTF("Init connections... \r\n");
  connections_initialize();

  NRF_LOG_PRINTF("Init GAP... \r\n");
  gap_initialize();

  NRF_LOG_PRINTF("Init GATT... \r\n");
  gatt_initialize();

#ifdef IS_PROD_BOARD
  NRF_LOG_PRINTF("Start BLE advertising... \r\n");
  start_advertising();
#else
  NRF_LOG_PRINTF("Start BLE scanning... \r\n");
  start_scanning();
#endif

  NRF_LOG_PRINTF("System ready.\r\n");
}

uint32_t run() {
  uint32_t error_code;

  terminal_process_input();
  app_sched_execute();

  sizeOfCurrentEvent = sizeOfEvent;
  error_code = sd_ble_evt_get(currentEventBuffer, &sizeOfCurrentEvent);

  if (error_code== NRF_SUCCESS) {
    handle_gap_event(currentEvent);
  }

  else if (error_code == NRF_ERROR_NOT_FOUND) {
    EC(sd_app_evt_wait());
    EC(sys_ClearPendingIRQ(SD_EVT_IRQn));

  } else {
    EC(error_code);
  }

  return NRF_SUCCESS;
}

#ifndef TESTING // exclude entry point from unit tests
int main()
{
  initialize();

  led_green_dim();

  for(;;) {
    run();
  }
}
#endif
