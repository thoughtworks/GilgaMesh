#include "app/feedback.h"

#include <system/util.h>
#include <system/timer.h>
#include <app/buzzer.h>
#include <command.h>
#include <app/votes.h>
#include "app/led.h"

#define FEEDBACK_REFRESH_RATE_IN_MS 100
#define MAX_LED_COLORS              6

SYS_TIMER_DEF(feedbackTimer);

static int rainbow_led_counter = 0;
static bool displaying_fun_feedback = false;
static bool initialization_completed = false;

static void display_general_user_feedback(void *data, uint16_t dataLength) {
  SYS_UNUSED_PARAMETER(data);
  SYS_UNUSED_PARAMETER(dataLength);

  if (buzzer_is_on()) {
    if (displaying_fun_feedback) {
      flash_color(rainbow_led_counter++);
      rainbow_led_counter %= MAX_LED_COLORS;
    }
    return;// do nothing, wait for buzzer to stop
  }

  displaying_fun_feedback = false;

//
//  if (get_nfc_status() == NFC_STATUS_ERROR) { // nfc is broken, display red
//    led_red_bright();
//
//  } else
  if (vote_storage_is_full()) {         // votes reached maximum, display red
    led_red_bright();

  } else if (!vote_config_is_set()) {   // no vote config, display dim red
    led_red_dim();

  } else {                              // neutral state, display dim blue
    led_blue_dim();
  }
}

void feedback_initialize() {
  create_repeated_timer(&feedbackTimer);
  start_timer(&feedbackTimer, FEEDBACK_REFRESH_RATE_IN_MS, display_general_user_feedback);

  mesh_add_terminal_command("fun", "Play a song!", display_fun_feedback);
}

void display_vote_recorded_feedback() {
  if(!initialization_completed) return;

  led_green_bright();
  uint16_t success_tones[4] = {50, 956, 50, 716};
  play_tones(success_tones, 2);
}

void display_vote_hit_recorded_feedback() {
  if(!initialization_completed) return;

  led_green_bright();
  uint16_t success_tones[4] = {25, 758};
  play_tones(success_tones, 1);
}

void display_successful_start_up_feedback() {
  led_white_bright();
  uint16_t hello_tones[8] = {200, 851, 200, 956, 200, 758, 300, 716};
  play_tones(hello_tones, 4);
  initialization_completed = true;
}

void display_group_value_change_feedback() {
  if(!initialization_completed) return;

  led_green_bright();
  uint16_t neutral_tones[2] = {500, 956};
  play_tones(neutral_tones, 1);
}

void display_failure_feedback() {
  led_red_bright();
  uint16_t failure_tones[4] = {200, 1073, 600, 1276};
  play_tones(failure_tones, 2);
}

void display_fun_feedback() {
  if(!initialization_completed) return;

  displaying_fun_feedback = true;
  rainbow_led_counter = 0;
  uint16_t countdown_tones[24] = {150, 902, 150, 1012, 500, 902, 1000, 1351, 300, 0, 150, 851, 150, 902, 200, 851, 100, 0, 200, 902, 100, 0, 600, 1012};
  play_tones(countdown_tones, 12);
}
