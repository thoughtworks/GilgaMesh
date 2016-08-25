#include <nrf51.h>
#include <nrf51_bitfields.h>
#include <app_pwm.h>
#include "cmocka_includes.h"

ret_code_t app_pwm_init(app_pwm_t const * const p_instance, app_pwm_config_t const * const p_config,
                        app_pwm_callback_t p_ready_callback) {
  check_expected_ptr(p_config);
  return NRF_SUCCESS;
}

ret_code_t app_pwm_uninit(app_pwm_t const * const p_instance) { }

void app_pwm_enable(app_pwm_t const * const p_instance) {
  check_expected_ptr(p_instance);
}

void app_pwm_disable(app_pwm_t const * const p_instance) { }

ret_code_t app_pwm_channel_duty_set(app_pwm_t const * const p_instance,
                                    uint8_t channel, app_pwm_duty_t duty) {
  check_expected(duty);
  return NRF_SUCCESS;
}