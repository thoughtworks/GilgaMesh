#include "app/buzzer.h"

#ifdef TESTING
#include <nrf51.h>
#include <nrf51_bitfields.h>
#endif
#include <app_pwm.h>
#include <stdlib.h>

#include "boards.h"
#include "system/timer.h"

#define MAX_SIZE_BUZZER_TONES_ARRAY   10
#define BUZZER_VOLUME                 50
#define DEFAULT_BUZZER_PERIOD         150

static buzzerTone tonesToPlay[MAX_SIZE_BUZZER_TONES_ARRAY];
static int numStoredTones = 0;
static int currentTone = 0;

static bool buzzerEnabled = false;
static bool buzzerPlaying = false;

static app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(DEFAULT_BUZZER_PERIOD, BUZZER_PIN_NUMBER);
APP_PWM_INSTANCE(PWM1, 1);
APP_TIMER_DEF(buzzerTimer);

void buzzer_initialize() {
  buzzerEnabled = true;

  create_single_shot_timer(&buzzerTimer);
  nrf_gpio_pin_clear(BUZZER_PIN_NUMBER);
}

static void resetTones() {
  memset(tonesToPlay, 0, sizeof(tonesToPlay));
  currentTone = 0;
  numStoredTones = 0;
}

static void appendNewBuzzerTones(uint16_t* toneInstructions, uint8_t numTones) {
    for (int i = 0; i < numTones; i++) {
    tonesToPlay[numStoredTones].duration = toneInstructions[i*2];
    tonesToPlay[numStoredTones].period = toneInstructions[i*2 + 1];
    numStoredTones++;
  }
}

static void add_pause() {
  tonesToPlay[numStoredTones].duration = 300;
  tonesToPlay[numStoredTones].period = 0;
  numStoredTones++;
}

static void turn_buzzer_off() {
  if(!buzzerEnabled) return;

  app_pwm_channel_duty_set(&PWM1, 0, 0);
  app_pwm_disable(&PWM1);
  app_pwm_uninit(&PWM1);
}

static void make_noise(void * p_event_data, uint16_t event_size) {
  UNUSED_PARAMETER(p_event_data);
  UNUSED_PARAMETER(event_size);

  if(!buzzerEnabled) return;
  turn_buzzer_off();

  if(currentTone < MAX_SIZE_BUZZER_TONES_ARRAY && (tonesToPlay[currentTone].duration > 0 || tonesToPlay[currentTone].period > 0)) {
    pwm_config.period_us = tonesToPlay[currentTone].period;
    EC(app_pwm_init(&PWM1, &pwm_config, NULL));
    app_pwm_enable(&PWM1);
    app_pwm_channel_duty_set(&PWM1, 0, BUZZER_VOLUME);

    start_timer(&buzzerTimer, tonesToPlay[currentTone++].duration, make_noise);
  }
  else {
    buzzerPlaying = false;
  }
}

void play_buzzer(uint16_t* toneInstructions, uint8_t numTones) {
  if(buzzerPlaying) {
    add_pause();
    appendNewBuzzerTones(toneInstructions, numTones);
  }
  else {
    resetTones();
    appendNewBuzzerTones(toneInstructions, numTones);
    buzzerPlaying = true;
    make_noise(NULL, NULL);
  }
}

bool buzzer_is_on() {
  return buzzerPlaying;
}
