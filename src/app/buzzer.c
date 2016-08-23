#include "app/buzzer.h"

#include <nrf.h>
#include <nrf_gpio.h>
#include <nrf_drv_config.h>
#include <app_pwm.h>
#include <stdlib.h>

#include "boards.h"
#include "system/timer.h"

#define MAX_NUM_BUZZER_TONES    3

static buzzerTone buzzes[MAX_NUM_BUZZER_TONES];
static int currentTone = 0;

static bool active = false;
static bool buzzerOn = false;

static app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(DEFAULT_BUZZER_PERIOD, BUZZER_PIN_NUMBER);
APP_PWM_INSTANCE(PWM1, 1);
APP_TIMER_DEF(buzzerTimer);

void buzzer_initialize() {
  active = true;

  create_single_shot_timer(&buzzerTimer);
  nrf_gpio_pin_clear(BUZZER_PIN_NUMBER);
}

void buzzer_on(char **parsedCommandArray, uint8_t numCommands) {
  memset(buzzes, 0, sizeof(buzzes));
  currentTone = 0;

  for(int i = 1; i < numCommands; i+=2) {
    buzzes[i/2].duration = (uint16_t) atoi(parsedCommandArray[i]);
    buzzes[i/2].period = (uint16_t) atoi(parsedCommandArray[i+1]);
  }

  buzzerOn = true;
  make_noise();
}

void make_noise() {
  if(!active) return;
  buzzer_off();

  if(currentTone < MAX_NUM_BUZZER_TONES && buzzes[currentTone].duration > 0 && buzzes[currentTone].period > 0) {
    pwm_config.period_us = buzzes[currentTone].period;
    EC(app_pwm_init(&PWM1, &pwm_config, NULL));
    app_pwm_enable(&PWM1);
    app_pwm_channel_duty_set(&PWM1, 0, BUZZER_VOLUME);

    start_timer(&buzzerTimer, buzzes[currentTone++].duration, make_noise);
  }
  else {
    buzzerOn = false;
  }
}

void buzzer_off() {
  if(!active) return;

  app_pwm_channel_duty_set(&PWM1, 0, 0);
  app_pwm_disable(&PWM1);
  app_pwm_uninit(&PWM1);
}

bool is_buzzer_on() {
  return buzzerOn;
}
