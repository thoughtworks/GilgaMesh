#include "terminal.h"
#include "cmocka_includes.h"

void terminal_putstring(char* string) {
  check_expected_ptr(string);
}

uint8_t terminal_get() {
  return mock_type(uint8_t);
}

bool terminal_initialized() {
  return mock_type(bool);
}

void terminal_put(const uint8_t character) { }
void terminal_get_with_timeout(uint8_t* character) { }
void terminal_clear() { }
void terminal_initialize(void) { }
void terminal_process_input(void) { }