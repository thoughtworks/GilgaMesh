#include <system/pwm.h>
#include "cmocka_includes.h"

const void* create_buzzer_pwm_instance() {
    return mock_type(void *);
}

const void* create_led_pwm_instance() {
    return mock_type(void *);
}

uint32_t sys_pwm_init(void* pwmInstance, uint32_t period, uint32_t pinNumber, bool highPolarity) {
    check_expected(pwmInstance);
    return 0;
}

uint32_t sys_pwm_uninit(void* pwmInstance) {
    return 0;
}

void sys_pwm_enable(void* pwmInstance) {
    check_expected(pwmInstance);
}

void sys_pwm_disable(void* pwmInstance) {
}

uint32_t sys_pwm_channel_duty_set(void* pwmInstance, sys_pwm_channel_id_t channel, sys_pwm_duty_t duty) {
    check_expected(pwmInstance);
    check_expected(channel);
    check_expected(duty);
    return 0;
}