#include "feedback.h"

#include <sched.h>
#include <sdk_common.h>

#include "buzzer.h"
#include "connection.h"
#include "led.h"

void display_general_user_feedback(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  if (is_buzzer_on()) return; // do nothing, wait for buzzer to stop

  led_white_off();

  if (get_nfc_status() == NFC_STATUS_ERROR) { // nfc is broken, display red
    led_red_on();

  } else if (vote_storage_is_full()) { // votes reached maximum, display yellow
    led_red_on();
    led_green_on();

  } else if (get_vote_count() > 0) { // votes are stored on the node, display teal
    led_blue_on();
    led_green_on();

  } else { // neutral state, display blue
    led_blue_on();
  }
}


void display_vote_recorded_feedback() {
  led_white_off();
  led_green_on();
  buzzer_on();
}


void display_group_value_change_feedback() {
  led_white_off();
  led_red_on();
  led_blue_on();
  buzzer_on();
}


void display_catastrophic_failure_feedback() {
  led_white_off();
  while(true) {
    led_red_on();
    nrf_delay_ms(100);
    led_red_off();
    led_blue_on();
    nrf_delay_ms(100);
    led_blue_off();
  }
}


void turn_off_buzzer_feedback(void *data, uint16_t dataLength) {
  led_white_off();
  buzzer_off();
  display_general_user_feedback(data, dataLength);
}

