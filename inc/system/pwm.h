#pragma once

#define DEFAULT_PWM_PERIOD         150
#define PWM_IO_PIN_NUMBER          6

#ifndef TESTING
    #include <app_pwm.h>
#endif

typedef uint8_t sys_pwm_channel_id_t;
typedef uint16_t sys_pwm_duty_t;

uint32_t sys_pwm_init();
uint32_t sys_pwm_uninit();
void sys_pwm_enable();
void sys_pwm_disable();
uint32_t sys_pwm_channel_duty_set(const sys_pwm_channel_id_t channel, const sys_pwm_duty_t duty);
void sys_pwm_set_default_period(sys_pwm_duty_t period_us);





