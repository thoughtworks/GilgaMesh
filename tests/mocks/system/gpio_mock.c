#include "cmocka_includes.h"

void sys_gpio_pin_clear(uint32_t pin_number) {
  check_expected(pin_number);
}

void sys_gpio_pin_set(uint32_t pin_number) {
  check_expected(pin_number);
}

void sys_gpio_cfg_output(uint32_t pin_number) {
  check_expected(pin_number);
}
