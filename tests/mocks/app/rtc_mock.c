
#include <stdbool.h>
#include <rtc.h>
#include "cmocka_includes.h"

void rtc_init() {}

void rtc_timestamp_to_timestring(timestamp_t stamp, timestring_t string) {
  for(int i = 0 ; i < TIMESTAMP_SIZE ; i++) {
    sprintf(string + 2 * i, "%02X", stamp[i]);
  }
  string[TIMESTRING_SIZE_INCLUDING_NULL - 1] = 0; // terminate with null
}

void rtc_timestring_to_timestamp(timestring_t string, timestamp_t stamp) {
  for(int i = 0 ; i < TIMESTAMP_SIZE ; i++) { // ignore the last (null) character
    stamp[i] = hex_digit_to_uint8(string[2 * i]) << 4; // tens
    stamp[i] += hex_digit_to_uint8(string[2 * i + 1]); // add units
  }
}


clock_time_t rtc_get_current_time() {

}

uint64_t rtc_get_seconds_since_boot() {
  return mock_type(uint64_t);
}

void rtc_print_status() {}

void rtc_set_field(char* field, char* value) {}

void rtc_print_date_and_time() {}

void rtc_get_timestamp(timestamp_t stamp) {
  rtc_timestring_to_timestamp(mock_type(char**), stamp);
}

void rtc_periodic_update_handler() {}