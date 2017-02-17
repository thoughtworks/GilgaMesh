#include <app_pwm.h>
#include "system/pwm.h"

APP_PWM_INSTANCE(PWM1, 1);
APP_PWM_INSTANCE(PWM2, 2);

const void *create_buzzer_pwm_instance() {
  return &PWM1;
}

const void *create_led_pwm_instance() {
  return &PWM2;
}

static app_pwm_config_t create_pwm_config_1ch(uint32_t period, uint32_t pinNumber, bool highPolarity) {
  app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(period, pinNumber);
  pwm_config.pin_polarity[0] = highPolarity ? APP_PWM_POLARITY_ACTIVE_HIGH : APP_PWM_POLARITY_ACTIVE_LOW;
  return pwm_config;
}

static app_pwm_config_t create_pwm_config_2ch(uint32_t period, uint32_t pinNumber1, uint32_t pinNumber2,
                                              bool highPolarity) {
  app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_2CH(period, pinNumber1, pinNumber2);
  pwm_config.pin_polarity[0] = highPolarity ? APP_PWM_POLARITY_ACTIVE_HIGH : APP_PWM_POLARITY_ACTIVE_LOW;
  pwm_config.pin_polarity[1] = highPolarity ? APP_PWM_POLARITY_ACTIVE_HIGH : APP_PWM_POLARITY_ACTIVE_LOW;
  return pwm_config;
}

uint32_t sys_pwm_init(void *pwmInstance, uint32_t period, bool highPolarity, uint32_t pinNumber1, uint32_t pinNumber2) {
  app_pwm_config_t pwmConfig;
  if (pinNumber2 != 0) {
    pwmConfig = create_pwm_config_2ch(period, pinNumber1, pinNumber2, highPolarity);
  } else {
    pwmConfig = create_pwm_config_1ch(period, pinNumber1, highPolarity);
  }
  return app_pwm_init(pwmInstance, &pwmConfig, NULL);
}

uint32_t sys_pwm_uninit(void *pwmInstance) {
  return app_pwm_uninit(pwmInstance);
}

void sys_pwm_enable(void *pwmInstance) {
  app_pwm_enable(pwmInstance);
}

void sys_pwm_disable(void *pwmInstance) {
  app_pwm_disable(pwmInstance);
}

uint32_t sys_pwm_channel_duty_set(void *pwmInstance, sys_pwm_channel_id_t channel, sys_pwm_duty_t duty) {
  return app_pwm_channel_duty_set(pwmInstance, channel, duty);
}
