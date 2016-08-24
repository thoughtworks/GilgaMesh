#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t duration;
  uint16_t period;
} __attribute__ ((packed)) buzzerTone;

void buzzer_initialize(void);
void play_buzzer(uint16_t* toneInstructions, uint8_t numTones);
bool buzzer_is_on(void);
