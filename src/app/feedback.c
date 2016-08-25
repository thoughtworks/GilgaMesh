#include "app/feedback.h"

#include <sdk_common.h>
#include <system/timer.h>
#include "app/led.h"

#define MS_RATE_TO_DISPLAY_USER_FEEDBACK 100

APP_TIMER_DEF(feedbackTimer);

void feedback_initialize() {
  create_repeated_timer(&feedbackTimer);
  start_timer(&feedbackTimer, APP_TIMER_TICKS(MS_RATE_TO_DISPLAY_USER_FEEDBACK, APP_TIMER_PRESCALER), display_general_user_feedback);
}

void display_general_user_feedback(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

//  if (buzzer_is_on()) return; // do nothing, wait for buzzer to stop
//
//  led_green_dim();

//  if (get_nfc_status() == NFC_STATUS_ERROR) { // nfc is broken, display red
//    led_red_bright();
//
//  } else if (vote_storage_is_full()) { // votes reached maximum, display yellow
//    led_red_bright();
//    led_green_bright();
//
//  } else if (get_vote_count() > 0) { // votes are stored on the node, display teal
//    led_blue_dim();
//    led_green_dim();
//
//  } else { // neutral state, display blue
//    led_blue_dim();
//  }
}


void display_vote_recorded_feedback() {
  led_all_off();
  led_green_bright();

#ifdef IS_PROD_BOARD
  uint16_t upward_progression_2[4] = {300, 2000, 300, 200};
  play_tones(upward_progression_2, 2);
#endif
}

void display_successful_start_up_feedback() {
  led_all_off();
  led_green_dim();
#ifdef IS_PROD_BOARD
  uint16_t robot_sound[6] = {300, 400, 300, 800, 300, 200};
  play_tones(robot_sound, 3);
#endif
}

void display_group_value_change_feedback() {
  led_all_off();
  led_red_bright();
  led_blue_bright();
#ifdef IS_PROD_BOARD
  uint16_t neutral_tone[1] = {120, 200};
  play_tones(neutral_tone, 1);
#endif
}


void display_catastrophic_failure_feedback() {
  led_all_off();
  led_red_bright();
#ifdef IS_PROD_BOARD
  uint16_t womp_womp[4] = {500, 500, 800, 3000};
  play_tones(womp_womp, 2);
#endif
}
