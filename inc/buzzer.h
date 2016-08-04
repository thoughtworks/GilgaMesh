#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef BOARD_SEEED_1_0
#define BUZZER_VOLUME 5
#else
#define BUZZER_VOLUME 50
#endif

bool active;
uint32_t pin_num;

void buzzer_initialize(void);
void buzzer_on(void);
void buzzer_off(void);
bool is_buzzer_on(void);

