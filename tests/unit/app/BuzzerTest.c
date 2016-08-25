#include "app/buzzer.h"
#include <nrf51.h>
#include <nrf51_bitfields.h>
#include <app_pwm.h>
#include "boards.h"

#include "cmocka_includes.h"

#define BUZZER_VOLUME                 50

static void setup_with_buzzer_enabled() {
  expect_any(create_single_shot_timer, timer_id);
  expect_any(nrf_gpio_pin_clear, pin_number);
  buzzer_initialize();
}

static int verifyPWMConfig(void *config1, void *config2) {
  app_pwm_config_t *actualConfig = (app_pwm_config_t *)config1;
  app_pwm_config_t *expectedConfig = (app_pwm_config_t *)config2;

  return (actualConfig->period_us == expectedConfig->period_us
          && actualConfig->pins == expectedConfig->pins
          && actualConfig->pin_polarity == expectedConfig->pin_polarity
          && actualConfig->num_of_channels == expectedConfig->num_of_channels) ? 0 : 1;
}


static void Buzzer_initialize_creates_timer_and_clears_pin() {
  expect_any(create_single_shot_timer, timer_id);
  expect_value(nrf_gpio_pin_clear, pin_number, BUZZER_PIN_NUMBER);

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
  app_pwm_config_t expectedConfig = APP_PWM_DEFAULT_CONFIG_1CH(period, BUZZER_PIN_NUMBER);

  expect_check(app_pwm_init, p_config, verifyPWMConfig, &expectedConfig);
  expect_any(app_pwm_enable, p_instance);
  expect_value(app_pwm_channel_duty_set, duty, BUZZER_VOLUME);
  expect_value(start_timer, ms_to_execute, duration);
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
