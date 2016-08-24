#include "cmocka_includes.h"

void nrf_gpio_pin_clear(uint32_t pin_number) {
  check_expected(pin_number);
}
