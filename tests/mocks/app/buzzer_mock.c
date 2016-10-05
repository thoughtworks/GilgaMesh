#include "app/buzzer.h"
#include "cmocka_includes.h"

bool buzzer_initialize(void) {
  return true;
}

void play_tones(uint16_t* toneInstructions, uint8_t numTones) {
  check_expected(toneInstructions);
  check_expected(numTones);
}

bool buzzer_is_on(void) {
  return mock_type(bool);
}

