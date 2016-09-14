//#include "app/led.h"
#include "cmocka_includes.h"

bool ledWhiteBright = false;
bool ledWhiteDim = false;

void led_initialize(void) { }
void led_red_bright(void) { }
void led_green_bright(void) { }
void led_blue_bright(void) { }
void led_purple_bright(void) { }
void led_teal_bright(void) { }
void led_yellow_bright(void) { }
void led_white_bright(void) {
  ledWhiteBright = true;
}
void led_white_dim(void) {
  ledWhiteDim = true;
}
void flash_color(int rainbow_led_counter) { }