#include <simple_uart.h>
#include "cmocka_includes.h"

static void console_print(const uint8_t* msg) {
#ifndef NO_LOG_OUTPUT
   printf(msg);
#endif
}

uint8_t simple_uart_get(void) {
   return mock_type(uint8_t);
}

bool simple_uart_get_with_timeout(int32_t timeout_us, uint8_t *rx_data) {
   check_expected(timeout_us);
   strcpy(rx_data, mock_type(uint8_t *));

   return mock_type(bool);
}

void simple_uart_put(uint8_t cr) {
   const uint8_t str[1] = { cr };
   console_print(str);
}


void simple_uart_putstring(const uint8_t *str) {
   console_print(str);
}

void simple_uart_config(uint8_t rts_pin_number, uint8_t txd_pin_number, uint8_t cts_pin_number, uint8_t rxd_pin_number, bool hwfc) {
   check_expected(rts_pin_number);
   check_expected(txd_pin_number);
   check_expected(cts_pin_number);
   check_expected(rxd_pin_number);
   check_expected(hwfc);
}


