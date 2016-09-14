#include "app/buzzer.h"
#include <system/pwm.h>
#include "boards.h"

#include "cmocka_includes.h"

#define BUZZER_VOLUME                 50

static void setup_with_buzzer_enabled() {
  expect_any(create_single_shot_timer, timer_id);
  expect_any(sys_gpio_pin_clear, pin_number);
  buzzer_initialize();
}

static void Buzzer_initialize_creates_timer_and_clears_pin() {
  expect_any(create_single_shot_timer, timer_id);
  expect_value(sys_gpio_pin_clear, pin_number, BUZZER_PIN_NUMBER);

  buzzer_initialize();
}

static void Buzzer_is_on_returns_false_when_buzzer_is_not_playing() {
  setup_with_buzzer_enabled();

  assert_false(buzzer_is_on());
}

static void Buzzer_play_tones_can_process_single_tone() {
  setup_with_buzzer_enabled();

  uint16_t duration = 500;
  uint16_t period = 250;
  uint16_t tones[1] = {duration, period};

  expect_value(sys_pwm_channel_duty_set, channel, 0);
  expect_value(sys_pwm_channel_duty_set, duty, BUZZER_VOLUME);
  expect_value(start_timer, ms_to_execute, duration);
  expect_any(start_timer, timer_id);
  play_tones(tones, 1);
}

int RunBuzzerTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(Buzzer_initialize_creates_timer_and_clears_pin),
          cmocka_unit_test(Buzzer_is_on_returns_false_when_buzzer_is_not_playing),
          cmocka_unit_test(Buzzer_play_tones_can_process_single_tone),
  };

  return cmocka_run_group_tests_name("BuzzerTest", tests, NULL, NULL);
}

int main() {
  return RunBuzzerTest();
}
