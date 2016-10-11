#include "app/led.h"

#include "system/gpio.h"
#include "system/log.h"
#include "system/pwm.h"
#include "boards.h"
#include "error.h"

#define LED_DEFAULT_PERIOD  10000

typedef void(*display_led_function)(void);

static const void* ledPWM;

static display_led_function flash[6] = {
        led_red_bright,
        led_yellow_bright,
        led_green_bright,
        led_teal_bright,
        led_blue_bright,
        led_purple_bright
};

static void set_non_pwm_led(uint32_t pinNumber, uint8_t dutyCycle) {
  if ((dutyCycle > 0 && INVERT_LEDS) || (dutyCycle == 0 && !INVERT_LEDS)) {
    sys_gpio_pin_set(pinNumber);
  } else {
    sys_gpio_pin_clear(pinNumber);
  }
}

static void set_rgb_leds(uint8_t redDutyCycle, uint8_t greenDutyCycle, uint8_t blueDutyCycle) {
  while (sys_pwm_channel_duty_set(ledPWM, 0, blueDutyCycle) == NRF_ERROR_BUSY);
  while (sys_pwm_channel_duty_set(ledPWM, 1, redDutyCycle) == NRF_ERROR_BUSY);

  set_non_pwm_led(BSP_LED_1, greenDutyCycle);
}

bool led_initialize() {
  ledPWM = create_led_pwm_instance();

  if (!execute_succeeds(sys_pwm_init(ledPWM, LED_DEFAULT_PERIOD, INVERT_LEDS, BSP_LED_2, BSP_LED_0))) return false;
  sys_pwm_enable(ledPWM);
  if (!execute_succeeds(sys_pwm_channel_duty_set(ledPWM, 0, LED_OFF_DUTY_CYCLE))) return false;
  if (!execute_succeeds(sys_pwm_channel_duty_set(ledPWM, 1, LED_OFF_DUTY_CYCLE))) return false;

  sys_gpio_cfg_output(BSP_LED_1);
  sys_gpio_pin_clear(BSP_LED_1);
  return true;
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

void led_blue_dim() {
  set_rgb_leds(LED_OFF_DUTY_CYCLE, LED_OFF_DUTY_CYCLE, LED_DIM_DUTY_CYCLE);
}

void led_red_dim() {
  set_rgb_leds(LED_DIM_DUTY_CYCLE, LED_OFF_DUTY_CYCLE, LED_OFF_DUTY_CYCLE);
}
