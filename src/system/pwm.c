#include "system/pwm.h"

APP_PWM_INSTANCE(PWM1, 1);

static app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(DEFAULT_PWM_PERIOD, PWM_IO_PIN_NUMBER);

uint32_t sys_pwm_init() {
    return app_pwm_init(&PWM1, &pwm_config, NULL);
}

uint32_t sys_pwm_uninit() {
    return app_pwm_uninit(&PWM1);
}

void sys_pwm_enable() {
    app_pwm_enable(&PWM1);
}

void sys_pwm_disable() {
    app_pwm_disable(&PWM1);
}

uint32_t sys_pwm_channel_duty_set(sys_pwm_channel_id_t channel, sys_pwm_duty_t duty) {
    return app_pwm_channel_duty_set(&PWM1, channel, duty);
}

void sys_pwm_set_default_period(sys_pwm_duty_t period_us) {
    pwm_config.period_us = period_us;
}