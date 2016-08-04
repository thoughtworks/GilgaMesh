#include "buzzer.h"

#include <nrf.h>
#include <nrf_gpio.h>
#include <nrf_drv_config.h>
#include <app_pwm.h>
#include <app_error.h>

#include "boards.h"
#include "timer.h"



static bool buzzerOn = false;

APP_PWM_INSTANCE(PWM1,1);

void pwm_ready_callback(uint32_t pwm_id) { }

void buzzer_initialize() {
  active = true;

  nrf_gpio_pin_clear(BUZZER_PIN_NUMBER);
  app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(150, BUZZER_PIN_NUMBER);
  EC(app_pwm_init(&PWM1, &pwm_config, pwm_ready_callback));
  app_pwm_enable(&PWM1);
}

void buzzer_on() {
  if(!active) return;

  app_pwm_channel_duty_set(&PWM1, 0, BUZZER_VOLUME);
  buzzerOn = true;
  turn_on_buzzer_feedback_timeout();
}

void buzzer_off() {
  if(!active) return;

  app_pwm_channel_duty_set(&PWM1, 0, 0);
  buzzerOn = false;
}

bool is_buzzer_on() {
  return buzzerOn;
}
