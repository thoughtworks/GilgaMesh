#include "app/buzzer.h"

#include <stdlib.h>
#include <string.h>

#include "system/gpio.h"
#include "system/pwm.h"
#include "system/timer.h"
#include "system/util.h"
#include "boards.h"

#define MAX_SIZE_BUZZER_TONES_ARRAY   20
#define BUZZER_VOLUME                 50

static buzzerTone tonesToPlay[MAX_SIZE_BUZZER_TONES_ARRAY];
static int numStoredTones = 0;
static int currentTone = 0;

static bool buzzerEnabled = false;
static bool buzzerPlaying = false;

SYS_TIMER_DEF(buzzerTimer);

static const void* buzzerPWM;

static void make_noise(void * p_event_data, uint16_t event_size);

bool buzzer_initialize() {
  if (execute_succeeds(create_single_shot_timer(&buzzerTimer))) {
    buzzerPWM = create_buzzer_pwm_instance();
    sys_gpio_pin_clear(BUZZER_PIN_NUMBER);
    buzzerEnabled = true;
  }

  return buzzerEnabled;
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

static void turn_buzzer_on() {
  EC(sys_pwm_init(buzzerPWM, tonesToPlay[currentTone].period, false, BUZZER_PIN_NUMBER, 0));
  sys_pwm_enable(buzzerPWM);
  EC(sys_pwm_channel_duty_set(buzzerPWM, 0, BUZZER_VOLUME));
  start_timer(&buzzerTimer, tonesToPlay[currentTone++].duration, make_noise);
  buzzerPlaying = true;
}

static void turn_buzzer_off() {
  sys_pwm_channel_duty_set(buzzerPWM, 0, 0);
  sys_pwm_disable(buzzerPWM);
  sys_pwm_uninit(buzzerPWM);
}

static void make_noise(void * p_event_data, uint16_t event_size) {
  SYS_UNUSED_PARAMETER(p_event_data);
  SYS_UNUSED_PARAMETER(event_size);

  if (buzzerPlaying) turn_buzzer_off();

  if(currentTone < MAX_SIZE_BUZZER_TONES_ARRAY && (tonesToPlay[currentTone].duration > 0 || tonesToPlay[currentTone].period > 0)) {
    turn_buzzer_on();
  } else {
    buzzerPlaying = false;
  }
}

void play_tones(uint16_t *toneInstructions, uint8_t numTones) {
  if(!buzzerEnabled) return;

  if(buzzerPlaying) {
    add_pause();
    appendNewBuzzerTones(toneInstructions, numTones);
  }
  else {
    resetTones();
    appendNewBuzzerTones(toneInstructions, numTones);

    make_noise(NULL, NULL);
  }
}

bool buzzer_is_on() {
  return buzzerPlaying;
}
