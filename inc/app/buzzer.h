#pragma once

#include <stdint.h>
#include <stdbool.h>

#define BUZZER_VOLUME 50
#define DEFAULT_BUZZER_PERIOD 150

typedef struct {
  uint16_t duration;
  uint16_t period;
} __attribute__ ((packed)) buzzerTone;

void buzzer_initialize(void);
void buzzer_on(char **parsedCommandArray, uint8_t numCommands);
void make_noise(void);
void buzzer_off(void);
bool is_buzzer_on(void);
