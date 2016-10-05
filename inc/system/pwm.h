#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifndef TESTING
    #include <app_pwm.h>
#endif

typedef uint8_t sys_pwm_channel_id_t;
typedef uint16_t sys_pwm_duty_t;

const void* create_buzzer_pwm_instance(void);
const void* create_led_pwm_instance(void);
uint32_t sys_pwm_init(void *pwmInstance, uint32_t period, bool highPolarity, uint32_t pinNumber1, uint32_t pinNumber2);
uint32_t sys_pwm_uninit(void* pwmInstance);
void sys_pwm_enable(void* pwmInstance);
void sys_pwm_disable(void* pwmInstance);
uint32_t sys_pwm_channel_duty_set(void* pwmInstance, const sys_pwm_channel_id_t channel, const sys_pwm_duty_t duty);





