#include <main.h>
#include <softdevice_handler_appsh.h>

#define APP_TIMER_PRESCALER         0                                  /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE     4                                  /**< Size of timer operation queues. */

// This variable ensures that the linker script will write the bootloader start
// address to the UICR register. This value will be written in the HEX file and
// thus written to UICR when MeshyMesh is flashed into the chip. */

volatile uint32_t m_uicr_bootloader_start_address  __attribute__ ((section(".uicrBootStartAddress"))) = BOOTLOADER_REGION_START;

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

void softdevice_assert_callback(uint32_t id, uint32_t pc, uint32_t info)
{
  for(;;) { }
}

static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{

}

uint32_t init_softdevice() {
  uint32_t err_code = NRF_SUCCESS;

  nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

  // Initialize SoftDevice.
  CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);
  NRF_LOG_PRINTF("  * SD handler init\r\n");
  SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

  ble_enable_params_t ble_enable_params;
  NRF_LOG_PRINTF("  * SD get default config\r\n");
  err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                  PERIPHERAL_LINK_COUNT,
                                                  &ble_enable_params);
  APP_ERROR_CHECK(err_code);

  // Enable BLE stack.
  NRF_LOG_PRINTF("  * SD enable\r\n");
  err_code = softdevice_enable(&ble_enable_params);
  APP_ERROR_CHECK(err_code);

  // Subscribe for BLE events.
  err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
  NRF_LOG_PRINTF("  * SD event subscribe\r\n");
  APP_ERROR_CHECK(err_code);

  return err_code;
}

void initialize() {
  NRF_LOG_PRINTF("Init timer... ");
  APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);
  NRF_LOG_PRINTF("Ok\r\n");

  NRF_LOG_PRINTF("Init buttons and leds... ");
  buttons_leds_init();
  NRF_LOG_PRINTF("Ok\r\n");

  NRF_LOG_PRINTF("Init softdevice... \r\n");
  init_softdevice();
  NRF_LOG_PRINTF("Softdevice initialized.\r\n");
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
