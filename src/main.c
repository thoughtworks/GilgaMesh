#include <main.h>
#include <pn532.h>

static uint16_t sizeOfCurrentEvent = sizeof(currentEventBuffer);

uint32_t initialize() {

#ifdef TIMER_DEBUG
  initialize_uart_nfc();
  timer_initialize();
#endif //TIMER_DEBUG

#ifndef NO_TERMINAL
  terminal_initialize();
#endif

  connections_initialize();
  ble_initialize();
  gatt_initialize();

  //Turn all LEDs off
  led_white_off();

  start_scanning();
  start_advertising();

  return NRF_SUCCESS;
}

uint32_t run() {
  uint32_t error_code;

#ifndef NO_TERMINAL
  terminal_process_input();
#endif

  sizeOfCurrentEvent = sizeOfEvent;
  error_code = sd_ble_evt_get(currentEventBuffer, &sizeOfCurrentEvent);

  if (error_code== NRF_SUCCESS) {
    handle_gap_event(currentEvent);
  }

  else if (error_code == NRF_ERROR_NOT_FOUND) {
    EC(sd_app_evt_wait());
    EC(sd_nvic_ClearPendingIRQ(SD_EVT_IRQn));

  } else {
    EC(error_code);
  }
  return NRF_SUCCESS;
}

int main(void)
{
  uint32_t error_code = initialize();

  if(error_code) {
     // TODO: Init failed, blink red led or something
  }
  else {
    while (!run()) { }
    // TODO: Run failed
  }
}
