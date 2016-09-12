#include <system/pwm.h>
#include "cmocka_includes.h"

uint32_t sys_pwm_init() {
    return 0;
}

uint32_t sys_pwm_uninit() {
    return 0;
}

void sys_pwm_enable() {}

void sys_pwm_disable() {}

uint32_t sys_pwm_channel_duty_set(const sys_pwm_channel_id_t channel, const sys_pwm_duty_t duty) {
    check_expected(channel);
    check_expected(duty);
    return 0;
}
void sys_pwm_set_default_period(sys_pwm_duty_t period_us) { }
