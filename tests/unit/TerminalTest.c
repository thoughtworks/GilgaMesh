#include "cmocka_includes.h"
#include "terminal.h"

// This is a dummy test
static void terminalTestOne(void **state) {
   (void) state; // unused

   assert_true(true);
}

static void terminalTestTwo(void **state) {
   (void) state; // unused

   assert_false(false);
}

int RunTerminalTest(void) {
   const struct CMUnitTest tests[] = {
      // list test functions here
      cmocka_unit_test(terminalTestOne),
      cmocka_unit_test(terminalTestTwo),
   };

   //set test suite name here
   return cmocka_run_group_tests_name("TerminalTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
   return RunTerminalTest();
}
