#include "cmocka_includes.h"
#include "rtc.h"
#include "time.h"
#include "conversion.h"

static void test_conversion(void **state) {
  (void) state; // unused
  timestring_t originalTime = "0F010B0203040100";
  timestring_t convertedTime;
  timestamp_t stamp;

  rtc_timestring_to_timestamp(originalTime, stamp);
  rtc_timestamp_to_timestring(stamp, convertedTime);

  assert_string_equal(convertedTime, originalTime);
}

static void test_comparison(void **state) {
  (void) state; // unused
  timestring_t originalTime = "0F01010203040100";
  timestring_t equalTime = "0F01010203040100";
  timestring_t unEqualTime = "4201010203040300";
  timestamp_t originalStamp, equalStamp, unEqualStamp;

  rtc_timestring_to_timestamp(originalTime, originalStamp);
  rtc_timestring_to_timestamp(equalTime, equalStamp);
  rtc_timestring_to_timestamp(unEqualTime, unEqualStamp);

  assert_true(rtc_is_equal_timestamp(originalTime, equalTime));
  assert_false(rtc_is_equal_timestamp(originalTime, unEqualTime));
}

int RunRTCTest(void) {
  const struct CMUnitTest tests[] = {
    // list test functions here
    cmocka_unit_test(test_conversion),
    cmocka_unit_test(test_comparison),
  };

  //set test suite name here
  return cmocka_run_group_tests_name("RTCTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
  return RunRTCTest();
}

