#pragma once

#include <stdint.h>
#include <stdbool.h>

bool active;
uint32_t pin_num;
uint32_t lastStateChangeMs;

void buzzer_initialize(void);
void buzzer_on(void);
void buzzer_off(void);
