#include "app/rtc.h"
#include "cmocka_includes.h"

void rtc_init() {}
clock_time_t rtc_get_current_time() {}

uint64_t rtc_get_seconds_since_boot() {
  return mock_type(uint64_t);
}

void rtc_print_status() {}
void rtc_set_field(char* field, char* value) {}
void rtc_print_date_and_time() {}
void rtc_get_timestamp(timestamp_t stamp) {}
void rtc_timestamp_to_timestring(timestamp_t stamp, timestring_t string) {}
void rtc_timestring_to_timestamp(timestring_t string, timestamp_t stamp) {
  check_expected(string);
  check_expected(stamp);
}

bool rtc_is_equal_timestamp(timestamp_t a, timestamp_t b) {
  return mock_type(bool);
}


void rtc_sysclock_timer_initialize() {}
void rtc_sysclock_timer_suspend(){}
void rtc_sysclock_timer_resume() {}
void rtc_periodic_update_handler() {}
void execute_rtc_command(char **parsedCommandArray) {}
void print_help_rtc() {}
void rtc_i2c_read_callback(uint32_t result, void* data) {}
