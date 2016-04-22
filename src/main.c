#include <main.h>

static uint16_t sizeOfCurrentEvent = sizeof(currentEventBuffer);

uint32_t initialize() {

#ifdef NFC_DEBUG
  initialize_uart_nfc();
  power_down();
  buzzer_initialize();
#endif

#ifndef NO_TERMINAL
  terminal_initialize();
#endif

  timer_initialize();

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

static void blink_red_blue_forever() {
  led_white_off();
  led_red_on();
  while(true) {
    led_blue_off();
    nrf_delay_ms(100);
    led_blue_on();
    nrf_delay_ms(100);
  }
}

int main(void)
{
  uint32_t error_code = initialize();

  if(error_code) {
     blink_red_blue_forever(); // Init failed
  }
  else {
    while (!run()) { }
    blink_red_blue_forever(); // Run failed
  }
}
