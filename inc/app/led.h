#pragma once

#include <nrf51.h>
#include <nrf51_bitfields.h>

#ifdef BOARD_SEEED
#define LED_BRIGHT_DUTY_CYCLE         0
#define LED_DIM_DUTY_CYCLE            250
#define LED_OFF_DUTY_CYCLE            255
#else
#define LED_BRIGHT_DUTY_CYCLE         255
#define LED_DIM_DUTY_CYCLE            5
#define LED_OFF_DUTY_CYCLE            0
#endif

void led_initialize(void);

void led_red_bright(void);
void led_red_dim(void);
void led_red_off(void);

void led_green_bright(void);
void led_green_dim(void);
void led_green_off(void);

void led_blue_bright(void);
void led_blue_dim(void);
void led_blue_off(void);

void led_white_bright(void);
void led_white_dim(void);
void led_white_off(void);

void led_all_off(void);