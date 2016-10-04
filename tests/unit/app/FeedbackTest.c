#include "app/feedback.h"
#include "cmocka_includes.h"

#define MS_RATE_TO_DISPLAY_USER_FEEDBACK 100

extern bool ledRedBright;
extern bool ledWhiteBright;
extern bool ledBlueDim;
extern void execute_timer_callback(bool value);

static int test_setup(void **state) {
  ledRedBright = false;
  ledWhiteBright = false;
  ledBlueDim = false;
  return 0;
}

static void Feedback_initialize_starts_timer_and_adds_terminal_command() {
  expect_any(create_repeated_timer, timer_id);
  expect_value(start_timer, ms_to_execute, MS_RATE_TO_DISPLAY_USER_FEEDBACK);
  expect_any(start_timer, timer_id);
  expect_string(mesh_add_terminal_command, commandName, "fun");

  feedback_initialize();
};

static void Feedback_initialize_displays_general_user_feedback() {
  execute_timer_callback(true);
  will_return(buzzer_is_on, false);
  will_return(vote_storage_is_full, false);
  expect_any(create_repeated_timer, timer_id);
  expect_any(start_timer, ms_to_execute);
  expect_any(start_timer, timer_id);
  expect_any(mesh_add_terminal_command, commandName);

  feedback_initialize();
  assert_true(ledBlueDim);
};

static void Feedback_initialize_displays_failure_when_vote_storage_is_full() {
  execute_timer_callback(true);
  will_return(buzzer_is_on, false);
  will_return(vote_storage_is_full, true);
  expect_any(create_repeated_timer, timer_id);
  expect_any(start_timer, ms_to_execute);
  expect_any(start_timer, timer_id);
  expect_any(mesh_add_terminal_command, commandName);

  feedback_initialize();
  assert_true(ledRedBright);
};

static void Feedback_initialize_does_not_display_feedback_if_buzzer_is_on() {
  execute_timer_callback(true);
  will_return(buzzer_is_on, true);
  expect_any(create_repeated_timer, timer_id);
  expect_any(start_timer, ms_to_execute);
  expect_any(start_timer, timer_id);
  expect_any(mesh_add_terminal_command, commandName);

  feedback_initialize();
  assert_false(ledBlueDim);
  assert_false(ledRedBright);
};

static void Feedback_display_successful_start_up_feedback_plays_four_tones_and_white_leds() {
  expect_any(play_tones, toneInstructions);
  expect_value(play_tones, numTones, 4);

  display_successful_start_up_feedback();
  assert_true(ledWhiteBright);
};

int RunFeedbackTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(Feedback_initialize_starts_timer_and_adds_terminal_command, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_displays_general_user_feedback, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_displays_failure_when_vote_storage_is_full, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_does_not_display_feedback_if_buzzer_is_on, test_setup),
          cmocka_unit_test_setup(Feedback_display_successful_start_up_feedback_plays_four_tones_and_white_leds, test_setup),
  };

  return cmocka_run_group_tests_name("FeedbackTest", tests, NULL, NULL);
}

int main() {
  return RunFeedbackTest();
}