#include "app/led.h"
#include <nrf_error.h>
#include "cmocka_includes.h"

static void Led_initialize_starts_all_pwm_instances() {
  will_return_always(low_power_pwm_init, NRF_SUCCESS);
  will_return_always(low_power_pwm_start, NRF_SUCCESS);

  expect_any_count(low_power_pwm_init, p_pwm_config, 3);
  expect_any_count(low_power_pwm_init, handler, 3);

  expect_any_count(low_power_pwm_start, p_pwm_instance, 3);
  expect_any_count(low_power_pwm_start, leds_pin_bit_mask, 3);

  led_initialize();
}

int RunLedTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(Led_initialize_starts_all_pwm_instances),
  };

  return cmocka_run_group_tests_name("LedTest", tests, NULL, NULL);
}

int main() {
  return RunLedTest();
}