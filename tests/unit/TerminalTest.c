#include "cmocka_includes.h"
#include "terminal.h"

// setup and teardown functions

static int GlobalSetup(void **state) {
   (void) state; // unused
   assert_false(terminal_initialized());

   expect_value(simple_uart_config, rts_pin_number, RTS_PIN_NUMBER);
   expect_value(simple_uart_config, txd_pin_number, TX_PIN_NUMBER);
   expect_value(simple_uart_config, cts_pin_number, CTS_PIN_NUMBER);
   expect_value(simple_uart_config, rxd_pin_number, RX_PIN_NUMBER);
   expect_value(simple_uart_config, hwfc, HWFC);

   will_return(dfu_device_name_with_id, "UnitTest");

   terminal_initialize();

   simple_uart_putstring((const uint8_t*) "Global setup\n");

   return 0;
}

// Tests

static void TerminalInitTest(void **state) {
   (void) state; // unused

   assert_true(terminal_initialized());
}

static void TerminalOutputsPromptOnEnter(void **state) {
   (void) state; // unused


   simple_uart_putstring((const uint8_t*) "TEST2\n");

   will_return(simple_uart_get_with_timeout, "\r");
   will_return(simple_uart_get_with_timeout, true);
   expect_value(simple_uart_get_with_timeout, timeout_100us, 1);

   terminal_process_input();
}

// Test runner

int RunTerminalTest(void) {
   const struct CMUnitTest tests[] = {
      // list test functions here
      cmocka_unit_test(TerminalInitTest),
      cmocka_unit_test(TerminalOutputsPromptOnEnter),
   };

   //set test suite name here
   return cmocka_run_group_tests_name("TerminalTest", tests, GlobalSetup, NULL);
}

int main(int argc, char **argv) {
   return RunTerminalTest();
}
