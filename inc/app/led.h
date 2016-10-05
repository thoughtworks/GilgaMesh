#pragma once

#include <nrf51.h>
#include <nrf51_bitfields.h>

#define LED_BRIGHT_DUTY_CYCLE         100
#define LED_DIM_DUTY_CYCLE            3
#define LED_OFF_DUTY_CYCLE            0

#ifndef INVERT_LEDS
#define INVERT_LEDS                   false
#endif

void led_initialize(void);

void led_red_bright(void);
void led_green_bright(void);
void led_blue_bright(void);
void led_purple_bright();
void led_teal_bright();
void led_yellow_bright();
void led_white_bright(void);
void led_blue_dim(void);
void led_red_dim(void);

void flash_color(int rainbow_led_counter);
