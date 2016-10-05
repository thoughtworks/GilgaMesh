#include "app/led.h"
#include <nrf_error.h>
#include "boards.h"
#include "cmocka_includes.h"

char *mockPWMInstance = "foo";

static void Led_initialize_starts_two_PWM_LEDs_and_one_GPIO_LED() {
  will_return(create_led_pwm_instance, mockPWMInstance);

  expect_value(sys_pwm_init, pwmInstance, mockPWMInstance);
  expect_value(sys_pwm_enable, pwmInstance, mockPWMInstance);
  expect_value_count(sys_pwm_channel_duty_set, pwmInstance, mockPWMInstance, 2);
  expect_value(sys_pwm_channel_duty_set, channel, 0);
  expect_value(sys_pwm_channel_duty_set, channel, 1);
  expect_value_count(sys_pwm_channel_duty_set, duty, LED_OFF_DUTY_CYCLE, 2);

  expect_value(sys_gpio_cfg_output, pin_number, BSP_LED_1);
  expect_value(sys_gpio_pin_clear, pin_number, BSP_LED_1);

  led_initialize();
}

int RunLedTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(Led_initialize_starts_two_PWM_LEDs_and_one_GPIO_LED),
  };

  return cmocka_run_group_tests_name("LedTest", tests, NULL, NULL);
}

int main() {
  return RunLedTest();
}