#include "app/led.h"
#include "cmocka_includes.h"

bool ledWhiteBright = false;
bool ledGreenDim = false;

void led_initialize(void) { }
void led_red_bright(void) { }
void led_red_dim(void) { }
void led_red_off(void) { }
void led_green_bright(void) { }
void led_green_dim(void) {
  ledGreenDim = true;
}
void led_green_off(void) { }
void led_blue_bright(void) { }
void led_blue_dim(void) { }
void led_blue_off(void) { }
void led_white_bright(void) {
  ledWhiteBright = true;
}
void led_white_dim(void) { }
void led_white_off(void) { }
void led_all_off(void) {
  ledGreenDim = false;
  ledWhiteBright = false;
}