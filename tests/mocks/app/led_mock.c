#include "cmocka_includes.h"

bool ledRedBright = false;
bool ledWhiteBright = false;
bool ledBlueDim = false;
bool ledRedDim = false;

void reset_leds() {
  ledRedBright = false;
  ledWhiteBright = false;
  ledBlueDim = false;
  ledRedDim = false;
}

bool led_initialize(void) {
  return true;
}

void led_red_bright(void) {
  ledRedBright = true;
}
void led_green_bright(void) { }
void led_blue_bright(void) { }
void led_purple_bright(void) { }
void led_teal_bright(void) { }
void led_yellow_bright(void) { }
void led_white_bright(void) {
  ledWhiteBright = true;
}
void led_blue_dim(void) {
  ledBlueDim = true;
}
void led_red_dim(void) {
  ledRedDim = true;
}
void flash_color(int rainbow_led_counter) { }