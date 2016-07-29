#include "led.h"

#include <low_power_pwm.h>
#include "boards.h"
#include "error.h"

static low_power_pwm_t led_pwm_red;
static low_power_pwm_t led_pwm_green;
static low_power_pwm_t led_pwm_blue;

void individual_led_initialize(low_power_pwm_t *pwm, const app_timer_id_t *timer, uint32_t mask) {
  low_power_pwm_config_t pwm_config = LOW_POWER_PWM_DEFAULT_CONFIG(mask);
  pwm_config.p_timer_id = timer;

  EC(low_power_pwm_init(pwm, &pwm_config, NULL));
//  EC(low_power_pwm_duty_set(pwm, LED_OFF_DUTY_CYCLE));
  EC(low_power_pwm_start(pwm, mask));
}

void led_initialize() {
  APP_TIMER_DEF(led_timer_red);
  APP_TIMER_DEF(led_timer_green);
  APP_TIMER_DEF(led_timer_blue);

  individual_led_initialize(&led_pwm_red,   &led_timer_red,   BSP_LED_0_MASK);
  individual_led_initialize(&led_pwm_green, &led_timer_green, BSP_LED_1_MASK);
  individual_led_initialize(&led_pwm_blue,  &led_timer_blue,  BSP_LED_2_MASK);
}

void led_red_bright() {
  EC(low_power_pwm_duty_set(&led_pwm_red, LED_BRIGHT_DUTY_CYCLE));
}

void led_red_dim() {
  EC(low_power_pwm_duty_set(&led_pwm_red, LED_DIM_DUTY_CYCLE));
}

void led_red_off() {
  EC(low_power_pwm_duty_set(&led_pwm_red, LED_OFF_DUTY_CYCLE));
}

void led_green_bright() {
  EC(low_power_pwm_duty_set(&led_pwm_green, LED_BRIGHT_DUTY_CYCLE));
}

void led_green_dim() {
  EC(low_power_pwm_duty_set(&led_pwm_green, LED_DIM_DUTY_CYCLE));
}

void led_green_off() {
  EC(low_power_pwm_duty_set(&led_pwm_green, LED_OFF_DUTY_CYCLE));
}

void led_blue_bright() {
  EC(low_power_pwm_duty_set(&led_pwm_blue, LED_BRIGHT_DUTY_CYCLE));
}

void led_blue_dim() {
  EC(low_power_pwm_duty_set(&led_pwm_blue, LED_DIM_DUTY_CYCLE));
}

void led_blue_off() {
  EC(low_power_pwm_duty_set(&led_pwm_blue, LED_OFF_DUTY_CYCLE));
}

void led_white_bright() {
  led_red_bright();
  led_green_bright();
  led_blue_bright();
}

void led_white_dim() {
  led_red_dim();
  led_green_dim();
  led_blue_dim();
}

void led_white_off() {
  led_red_off();
  led_green_off();
  led_blue_off();
}