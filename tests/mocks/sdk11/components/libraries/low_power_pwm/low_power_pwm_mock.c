#include <nrf51.h>
#include <nrf51_bitfields.h>
#include <low_power_pwm.h>
#include "cmocka_includes.h"

ret_code_t low_power_pwm_init(low_power_pwm_t * p_pwm_instance,
                              low_power_pwm_config_t const * p_pwm_config,
                              app_timer_timeout_handler_t handler) {
  check_expected_ptr(p_pwm_config);
  check_expected(handler);
  return mock_type(ret_code_t);
}

ret_code_t low_power_pwm_start(low_power_pwm_t * p_pwm_instance,
                               uint32_t leds_pin_bit_mask) {
  check_expected_ptr(p_pwm_instance);
  check_expected(leds_pin_bit_mask);
  return mock_type(ret_code_t);
}

ret_code_t low_power_pwm_duty_set(low_power_pwm_t * p_pwm_instance,
                                  uint8_t duty_cycle) {
  check_expected_ptr(p_pwm_instance);
  check_expected(duty_cycle);
  return mock_type(ret_code_t);
}
