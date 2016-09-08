#include "app/led.h"

#include <low_power_pwm.h>
#include "boards.h"
#include "error.h"

typedef void(*display_led_function)(void);
static low_power_pwm_t led_pwm_red;
static low_power_pwm_t led_pwm_green;
static low_power_pwm_t led_pwm_blue;

static display_led_function flash[6] = {
        led_red_bright,
        led_yellow_bright,
        led_green_bright,
        led_teal_bright,
        led_blue_bright,
        led_purple_bright
};

static void set_rgb_leds(uint8_t redValue, uint8_t greenValue, uint8_t blueValue) {
  EC(low_power_pwm_duty_set(&led_pwm_red, redValue));
  EC(low_power_pwm_duty_set(&led_pwm_green, greenValue));
  EC(low_power_pwm_duty_set(&led_pwm_blue, blueValue));
}

static void individual_led_initialize(low_power_pwm_t *pwm, const app_timer_id_t *timer, uint32_t mask) {
  low_power_pwm_config_t pwm_config = LOW_POWER_PWM_DEFAULT_CONFIG(mask);
  pwm_config.p_timer_id = timer;

  EC(low_power_pwm_init(pwm, &pwm_config, NULL));
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

void flash_color(int rainbow_led_counter) {
  flash[rainbow_led_counter]();
}

void led_red_bright() {
  set_rgb_leds(LED_BRIGHT_DUTY_CYCLE, LED_OFF_DUTY_CYCLE, LED_OFF_DUTY_CYCLE);
}

void led_green_bright() {
  set_rgb_leds(LED_OFF_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE, LED_OFF_DUTY_CYCLE);
}

void led_blue_bright() {
  set_rgb_leds(LED_OFF_DUTY_CYCLE, LED_OFF_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE);
}

void led_purple_bright() {
  set_rgb_leds(LED_BRIGHT_DUTY_CYCLE, LED_OFF_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE);
}

void led_teal_bright() {
  set_rgb_leds(LED_OFF_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE);
}

void led_yellow_bright() {
  set_rgb_leds(LED_BRIGHT_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE, LED_OFF_DUTY_CYCLE);
}

void led_white_bright() {
  set_rgb_leds(LED_BRIGHT_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE, LED_BRIGHT_DUTY_CYCLE);
}

void led_white_dim() {
  set_rgb_leds(LED_DIM_DUTY_CYCLE, LED_DIM_DUTY_CYCLE, LED_DIM_DUTY_CYCLE);
}
