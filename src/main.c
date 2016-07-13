#include <main.h>
#include <softdevice_handler_appsh.h>
#include <app_timer_appsh.h>
#include <app_scheduler.h>
#include <terminal.h>

#define APP_TIMER_PRESCALER         0                                  /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE     4                                  /**< Size of timer operation queues. */

// This variable ensures that the linker script will write the bootloader start
// address to the UICR register. This value will be written in the HEX file and
// thus written to UICR when MeshyMesh is flashed into the chip. */

#ifndef TESTING
volatile uint32_t m_uicr_bootloader_start_address  __attribute__ ((section(".uicrBootStartAddress"))) = BOOTLOADER_REGION_START;
#endif

static void bsp_event_handler(bsp_event_t event)
{
  switch (event)
  {
    case BSP_EVENT_KEY_0:
      NRF_LOG_PRINTF("BSP_EVENT_KEY_0\n");
        break;

    case BSP_EVENT_KEY_1:
      NRF_LOG_PRINTF("BSP_EVENT_KEY_1\n");
        break;

    default:
      break;
  }
}

static void buttons_leds_init()
{
  ret_code_t err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS,
                                 APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),
                                 bsp_event_handler);
  APP_ERROR_CHECK(err_code);
}

void panic()
{
  bsp_indication_set(BSP_INDICATE_FATAL_ERROR);
  for(;;) { }
}


void softdevice_fault_callback(uint32_t id, uint32_t pc, uint32_t info)
{
  for(;;) { }
}

static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{

}

static void sys_evt_dispatch(uint32_t sys_evt)
{

}

uint32_t init_softdevice() {
  uint32_t err_code = NRF_SUCCESS;
  sd_mbr_command_t com = {SD_MBR_COMMAND_INIT_SD, };
  nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

  // Initialize SoftDevice.
  NRF_LOG_PRINTF("  * mbr initialization\r\n");
  err_code = sd_mbr_command(&com);
  APP_ERROR_CHECK(err_code);

  NRF_LOG_PRINTF("  * vector table\r\n");
  err_code = sd_softdevice_vector_table_base_set(BOOTLOADER_REGION_START);
  APP_ERROR_CHECK(err_code);

  // Initialize the SoftDevice handler module.
  NRF_LOG_PRINTF("  * softdevice handler\r\n");
  SOFTDEVICE_HANDLER_APPSH_INIT(&clock_lf_cfg, true);

  ble_enable_params_t ble_enable_params;
  NRF_LOG_PRINTF("  * BLE params: maximum of %d central and %d periheral connections\r\n", CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);
  err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                  PERIPHERAL_LINK_COUNT,
                                                  &ble_enable_params);
  APP_ERROR_CHECK(err_code);

  // Enable BLE stack.
  NRF_LOG_PRINTF("  * enable softdevice\r\n");
  err_code = softdevice_enable(&ble_enable_params);
  APP_ERROR_CHECK(err_code);

  // Register with the SoftDevice handler module for BLE events.
  NRF_LOG_PRINTF("  * register BLE event handler\r\n");
  err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
  APP_ERROR_CHECK(err_code);

  // Register with the SoftDevice handler module for BLE events.
  NRF_LOG_PRINTF("  * register system event handler\r\n");
  err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
  APP_ERROR_CHECK(err_code);

  return err_code;
}

void initialize() {

//  NRF_LOG_PRINTF("Init scheduler... ");
//  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
//
//  NRF_LOG_PRINTF("Init timers... ");
//  APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);

  NRF_LOG_PRINTF("Init buttons and leds... ");
  buttons_leds_init();

  NRF_LOG_PRINTF("Init softdevice... \r\n");
  init_softdevice();

  NRF_LOG_PRINTF("Init terminal... \r\n");
  terminal_initialize();

  NRF_LOG_PRINTF("Init command... \r\n");
  command_initialize();
}

static void power_manage(void)
{
  ret_code_t err_code = sd_app_evt_wait();
  APP_ERROR_CHECK(err_code);
}

void boot() {
  NRF_LOG_INIT();
  NRF_LOG_PRINTF("\r\n[[ MeshyMesh is booting ]]\r\n");
  initialize();
  NRF_LOG_PRINTF("System ready.\r\n");
}

uint32_t run() {
  power_manage();

  return NRF_SUCCESS;
}

#ifndef TESTING // exclude entry point from unit tests
int main()
{
  boot();

  for(;;) {
    run();
  }
}
#endif
