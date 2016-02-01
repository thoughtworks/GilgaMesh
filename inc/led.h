#pragma once

#include <stdint.h>
#include <pca10028.h>
#include "nrf.h"

#define LED_RED    BSP_LED_0_MASK
#define LED_GREEN  BSP_LED_1_MASK
#define LED_BLUE   BSP_LED_2_MASK
#define LED_WHITE  (BSP_LED_0_MASK | BSP_LED_1_MASK | BSP_LED_2_MASK)

void led_red_on(void);
void led_red_off(void);
void led_green_on(void);
void led_green_on(void);
void led_blue_off(void);
void led_blue_off(void);
void led_white_off(void);
void led_white_off(void);

