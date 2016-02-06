#include <main.h>

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

    } else {
      APP_ERROR_CHECK(err);
    }
  }
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
