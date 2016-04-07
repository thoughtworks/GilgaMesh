#include <simple_uart.h>
#include "cmocka_includes.h"

uint8_t simple_uart_get(void) {
   return mock_type(uint8_t);
}


bool simple_uart_get_with_timeout(int32_t timeout_ms, uint8_t *rx_data) {
   check_expected(timeout_ms);
   check_expected(rx_data);

   return mock_type(bool);
}

void simple_uart_put(uint8_t cr) {
   check_expected(cr);
}


void simple_uart_putstring(const uint8_t *str) {
   check_expected(str);
}

void simple_uart_config(uint8_t rts_pin_number, uint8_t txd_pin_number, uint8_t cts_pin_number, uint8_t rxd_pin_number, bool hwfc) {
   check_expected(rts_pin_number);
   check_expected(txd_pin_number);
   check_expected(cts_pin_number);
   check_expected(rxd_pin_number);
   check_expected(hwfc);
}


