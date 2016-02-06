#include <main.h>

static uint8_t currentEventBuffer[sizeof(ble_evt_t) + BLE_L2CAP_MTU_DEF];
static ble_evt_t* currentEvent = (ble_evt_t *) currentEventBuffer;
static const uint16_t sizeOfEvent = sizeof(currentEventBuffer);
static uint16_t sizeOfCurrentEvent = sizeof(currentEventBuffer);


int main(void)
{
  uint32_t err;

  terminal_initialize();
  connections_initialize();
  ble_initialize();
  timer_initialize();

  //Turn all LEDs off
  led_white_off();

  start_scanning();
  start_advertising();

  while (true)
  {
    uint32_t err = NRF_ERROR_NOT_FOUND;
    //***** Terminal::PollUART();

    do {
      sizeOfCurrentEvent = sizeOfEvent;
      err = sd_ble_evt_get(currentEventBuffer, &sizeOfCurrentEvent);

      if (err == NRF_SUCCESS) {
        handle_gap_event(currentEvent);
      }

      else if (err == NRF_ERROR_NOT_FOUND) {
        //timer_tick_handler();

        err = sd_app_evt_wait();
        APP_ERROR_CHECK(err);
        sd_nvic_ClearPendingIRQ(SD_EVT_IRQn);
        break;

      } else {
        APP_ERROR_CHECK(err);
        break;
      }
    } while (true);
  }
}


void sys_evt_dispatch(uint32_t sys_evt)
{
  pstorage_sys_event_handler(sys_evt);
}


void ble_initialize(void){
  uint32_t err = 0;

  log("NODE: Initializing Softdevice");

  err = softdevice_handler_init(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, currentEventBuffer, sizeOfEvent, NULL);
  APP_ERROR_CHECK(err);

  err = softdevice_sys_evt_handler_set(sys_evt_dispatch);
  APP_ERROR_CHECK(err);

  ble_enable_params_t bleSdEnableParams;
  memset(&bleSdEnableParams, 0, sizeof(bleSdEnableParams));
  bleSdEnableParams.gatts_enable_params.attr_tab_size = ATTR_TABLE_MAX_SIZE;
  bleSdEnableParams.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;

  err = sd_ble_enable(&bleSdEnableParams);
  APP_ERROR_CHECK(err);

  err = sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
  APP_ERROR_CHECK(err);

  err = sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
  APP_ERROR_CHECK(err);


  ble_gap_conn_sec_mode_t secPermissionOpen;
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&secPermissionOpen);

  err = sd_ble_gap_device_name_set(&secPermissionOpen, (uint8_t*)MESH_NAME, strlen(MESH_NAME));
  APP_ERROR_CHECK(err);

  err = sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_COMPUTER);
  APP_ERROR_CHECK(err);

  err = sd_ble_gap_ppcp_set(&meshConnectionParams);
  APP_ERROR_CHECK(err);
}


void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
  if(error_code != NRF_SUCCESS){
    const char* error_string = getNrfErrorString(error_code);
    log("ERROR: ERROR CODE %d: %s, in file %s@%d", error_code, error_string, p_file_name, line_num);
  }

  if (error_code == NRF_ERROR_INVALID_STATE){
    return;
  }

  if (error_code == NRF_ERROR_BUSY) {
    return;
  }
}


void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
  app_error_handler(0xDEADBEEF, 0, NULL);
}

void HardFault_Handler(void)
{
  for (;;){
    // Endless debugger loop
  }
}
