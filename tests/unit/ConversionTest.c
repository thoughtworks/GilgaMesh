#include "cmocka_includes.h"
#include "conversion.h"

static void convertZeroToString(void **state) {
  (void) state; // unused

  assert_string_equal("0", int_to_string(0));
}

static void convertOneToString(void **state) {
  (void) state; // unused

  assert_string_equal("1", int_to_string(1));
}

static void convertLargeNumberToString(void **state) {
  (void) state; // unused

  assert_string_equal("12345", int_to_string(12345));
}

int RunConversionTest(void) {
  const struct CMUnitTest tests[] = {
          // list test functions here
          cmocka_unit_test(convertZeroToString),
          cmocka_unit_test(convertOneToString),
          cmocka_unit_test(convertLargeNumberToString)
  };

  //set test suite name here
  return cmocka_run_group_tests_name("ConversionTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
  return RunConversionTest();
}