#include "app/feedback.h"

#include "app/nfc.h"
#include "cmocka_includes.h"

#define MS_RATE_TO_DISPLAY_USER_FEEDBACK 100

extern bool ledBlueDim;
extern bool ledRedDim;
extern bool ledRedBright;
extern bool ledWhiteBright;
extern void execute_timer_callback(bool value);

static int test_setup(void **state) {
  reset_leds();
  return 0;
}

static void initialize_with_expectations() {
  expect_any(create_repeated_timer, timer_id);
  expect_any(start_timer, ms_to_execute);
  expect_any(start_timer, timer_id);
  expect_any(mesh_add_terminal_command, commandName);
  execute_timer_callback(true);

  feedback_initialize();
}

static void successful_startup_with_expectations() {
  expect_any(play_tones, toneInstructions);
  expect_any(play_tones, numTones);

  display_successful_start_up_feedback();
}

static void Feedback_initialize_starts_timer_and_adds_terminal_command() {
  expect_any(create_repeated_timer, timer_id);
  expect_value(start_timer, ms_to_execute, MS_RATE_TO_DISPLAY_USER_FEEDBACK);
  expect_any(start_timer, timer_id);
  expect_string(mesh_add_terminal_command, commandName, "fun");

  feedback_initialize();
};

static void Feedback_initialize_does_not_display_feedback_if_buzzer_is_on() {
  will_return(buzzer_is_on, true);

  initialize_with_expectations();

  assert_false(ledBlueDim);
  assert_false(ledRedDim);
  assert_false(ledRedBright);
  assert_false(ledWhiteBright);
};

static void Feedback_initialize_displays_failure_when_startup_failed() {
  will_return(buzzer_is_on, false);

  // no successful startup with expectations
  initialize_with_expectations();

  assert_true(ledRedBright);
};

static void Feedback_initialize_displays_failure_when_nfc_is_in_error_state() {
  will_return(buzzer_is_on, false);
  will_return(get_nfc_status, NFC_STATUS_ERROR);

  successful_startup_with_expectations();
  initialize_with_expectations();

  assert_true(ledRedBright);
};

static void Feedback_initialize_displays_failure_when_vote_storage_is_full() {
  will_return(buzzer_is_on, false);
  will_return(get_nfc_status, NFC_STATUS_WORKING);
  will_return(vote_storage_is_full, true);

  successful_startup_with_expectations();
  initialize_with_expectations();

  assert_true(ledRedBright);
};

static void Feedback_initialize_displays_unready_state_when_vote_config_is_missing() {
  will_return(buzzer_is_on, false);
  will_return(get_nfc_status, NFC_STATUS_WORKING);
  will_return(vote_storage_is_full, false);
  will_return(vote_config_is_set, false);

  successful_startup_with_expectations();
  initialize_with_expectations();

  assert_true(ledRedDim);
};

static void Feedback_initialize_displays_success_when_no_errors_occur() {
  will_return(buzzer_is_on, false);
  will_return(get_nfc_status, NFC_STATUS_WORKING);
  will_return(vote_storage_is_full, false);
  will_return(vote_config_is_set, true);

  successful_startup_with_expectations();
  initialize_with_expectations();

  assert_true(ledBlueDim);
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
          cmocka_unit_test_setup(Feedback_initialize_does_not_display_feedback_if_buzzer_is_on, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_displays_failure_when_startup_failed, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_displays_failure_when_nfc_is_in_error_state, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_displays_failure_when_vote_storage_is_full, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_displays_unready_state_when_vote_config_is_missing, test_setup),
          cmocka_unit_test_setup(Feedback_initialize_displays_success_when_no_errors_occur, test_setup),
          cmocka_unit_test_setup(Feedback_display_successful_start_up_feedback_plays_four_tones_and_white_leds, test_setup),
  };

  return cmocka_run_group_tests_name("FeedbackTest", tests, NULL, NULL);
}

int main() {
  return RunFeedbackTest();
}