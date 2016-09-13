#include "system/log.h"

static bool user_input_mode;

void set_user_input_mode(bool newValue) {
  user_input_mode = newValue;
}

bool in_user_input_mode() {
  return user_input_mode;
}