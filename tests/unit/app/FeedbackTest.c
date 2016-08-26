#include "app/feedback.h"
#include "cmocka_includes.h"

#define MS_RATE_TO_DISPLAY_USER_FEEDBACK 100

extern bool ledWhiteBright;
extern bool ledGreenDim;
extern void execute_timer_callback(bool value);

static void Feedback_display_successful_start_up_feedback_plays_buzzer_and_white_leds() {
  expect_any(play_tones, toneInstructions);
  expect_value(play_tones, numTones, 3);

  display_successful_start_up_feedback();
  assert_true(ledWhiteBright);
};

static void Feedback_initialize_creates_and_starts_timer() {
  expect_any(create_repeated_timer, timer_id);
  expect_value(start_timer, ms_to_execute, MS_RATE_TO_DISPLAY_USER_FEEDBACK);
  expect_any(start_timer, timer_id);

  feedback_initialize();
};

static void Feedback_initialize_displays_general_user_feedback() {
  led_all_off();
  execute_timer_callback(true);
  will_return(buzzer_is_on, false);
  expect_any(create_repeated_timer, timer_id);
  expect_any(start_timer, ms_to_execute);
  expect_any(start_timer, timer_id);

  feedback_initialize();
  assert_true(ledGreenDim);
};

static void Feedback_initialize_does_not_display_feedback_if_buzzer_is_on() {
  led_all_off();
  execute_timer_callback(true);
  will_return(buzzer_is_on, true);
  expect_any(create_repeated_timer, timer_id);
  expect_any(start_timer, ms_to_execute);
  expect_any(start_timer, timer_id);

  feedback_initialize();
  assert_false(ledGreenDim);
};

int RunFeedbackTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(Feedback_display_successful_start_up_feedback_plays_buzzer_and_white_leds),
          cmocka_unit_test(Feedback_initialize_creates_and_starts_timer),
          cmocka_unit_test(Feedback_initialize_displays_general_user_feedback),
          cmocka_unit_test(Feedback_initialize_does_not_display_feedback_if_buzzer_is_on),
  };

  return cmocka_run_group_tests_name("FeedbackTest", tests, NULL, NULL);
}

int main() {
  return RunFeedbackTest();
}