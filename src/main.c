#include <main.h>

static uint16_t sizeOfCurrentEvent = sizeof(currentEventBuffer);


int main(void)
{
  uint32_t error_code;

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
    uint32_t error_code = NRF_ERROR_NOT_FOUND;
    //***** Terminal::PollUART();

    sizeOfCurrentEvent = sizeOfEvent;
    error_code = sd_ble_evt_get(currentEventBuffer, &sizeOfCurrentEvent);

    if (error_code== NRF_SUCCESS) {
      handle_gap_event(currentEvent);
    }

    else if (error_code == NRF_ERROR_NOT_FOUND) {
      //timer_tick_handler();

      EC(sd_app_evt_wait());
      EC(sd_nvic_ClearPendingIRQ(SD_EVT_IRQn));

    } else {
      EC(error_code);
    }
  }
}
