#include <nrf.h>
#include <nrf_gpio.h>
#include <nrf_drv_config.h>
#include <app_pwm.h>
#include <app_error.h>

#include "buzzer.h"

APP_PWM_INSTANCE(PWM1,1);

void pwm_ready_callback(uint32_t pwm_id)
{
}

void buzzer_initialize() {
  active = true;
  pin_num = 6;
  lastStateChangeMs = 0;
}

void buzzer_on() {
  if(!active) return;

  app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(150, pin_num);
  APP_ERROR_CHECK(app_pwm_init(&PWM1, &pwm_config, pwm_ready_callback));
  app_pwm_enable(&PWM1);
  app_pwm_channel_duty_set(&PWM1, 0, 5);
}

void buzzer_off() {
  if(!active) return;

  app_pwm_channel_duty_set(&PWM1, 0, 0);
  app_pwm_disable(&PWM1);
  app_pwm_uninit(&PWM1);
  nrf_gpio_pin_clear(6);
}
