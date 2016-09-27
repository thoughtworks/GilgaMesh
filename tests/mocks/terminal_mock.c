#include "terminal.h"
#include "cmocka_includes.h"

void terminal_initialize(void) { }
void terminal_putstring(char* string) {
  check_expected_ptr(string);
}

uint8_t terminal_get() {
  return mock_type(uint8_t);
}

void terminal_process_input(void) { }
void log_to_uart(char *logMessage, uint16_t messageLength) { }