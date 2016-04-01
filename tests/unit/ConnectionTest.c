#include "cmocka_includes.h"

// This is a dummy test
static void connectionTestOne(void **state) {
   (void) state; // unused

   assert_true(true);
}

static void connectionTestTwo(void **state) {
   (void) state; // unused

   assert_false(false);
}

int RunConnectionTest(void) {
   const struct CMUnitTest tests[] = {
      // list test functions here
      cmocka_unit_test(connectionTestOne),
      cmocka_unit_test(connectionTestTwo),
   };

   //set test suite name here
   return cmocka_run_group_tests_name("ConnectionTest", tests, NULL, NULL);
}

