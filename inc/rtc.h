#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t millis;
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t day;
  uint8_t month;
  uint16_t year;
  uint8_t weekday;
  uint8_t years_since_y2k;
  int16_t UTC_offset_in_minutes;
} __attribute__ ((packed)) clock_time_t;

typedef struct {
  uint64_t seconds_since_boot;
  clock_time_t clock_time;
} __attribute__ ((packed)) system_time_t;

static system_time_t system_time;

typedef struct
{
  union {
    uint8_t Control_1;
    struct {
      uint8_t CAP_SEL : 1;
      uint8_t AM_PM : 1;
      uint8_t CIE : 1;
      uint8_t : 1;
      uint8_t SR : 1;
      uint8_t STOP : 1;
      uint8_t : 1;
      uint8_t EXT_TEST : 1;
    };
  } register_0;
  union {
    uint8_t Control_2;
    struct {
      uint8_t COF : 3;
      uint8_t TF: 1;
      uint8_t HMI : 1;
      uint8_t MI : 1;
      uint8_t : 2;
    };
  } register_1;
  union {
    int8_t Offset;
  } register_2;
  union {
    uint8_t RAM_byte;
  } register_3;
  union {
    uint8_t Seconds;
    struct {
      uint8_t UNIT : 4;
      uint8_t TENS : 3;
      uint8_t OS : 1;
    };
  } register_4;
  union {
    uint8_t Minutes;
    struct {
      uint8_t UNIT : 4;
      uint8_t TENS : 3;
      uint8_t : 1;
    };
  } register_5;
  union {
    uint8_t Hours;
    union {
      struct {
        uint8_t UNIT : 4;
        uint8_t TENS : 2;
        uint8_t : 2;
      };
      struct {
        uint8_t AMPM_UNIT : 4;
        uint8_t AMPM_TENS : 1;
        uint8_t IS_PM : 1;
        uint8_t : 2;
      };
    };
  } register_6;
  union {
    uint8_t Days;
    struct {
      uint8_t UNIT : 4;
      uint8_t TENS : 2;
      uint8_t : 2;
    };
  } register_7;
  union {
    uint8_t Weekdays;
    struct {
      uint8_t UNIT : 3;
      uint8_t : 5;
    };
  } register_8;
  union {
    uint8_t Months;
    struct {
      uint8_t UNIT : 4;
      uint8_t TENS : 1;
      uint8_t : 3;
    };
  } register_9;
  union {
    uint8_t Years;
    struct {
      uint8_t UNIT : 4;
      uint8_t TENS : 4;
    };
  } register_A;
}__attribute__ ((packed)) PCF85063TPState_t;

#define TIMESTAMP_SIZE 8
typedef uint8_t timestamp_t[TIMESTAMP_SIZE];
#define TIMESTRING_SIZE_INCLUDING_NULL 17
typedef char timestring_t[TIMESTRING_SIZE_INCLUDING_NULL];

void rtc_init();
clock_time_t rtc_get_current_time();
uint64_t rtc_get_seconds_since_boot();
void rtc_print_status();
void rtc_set_field(char* field, char* value);
void rtc_print_date_and_time();
void rtc_get_timestamp(timestamp_t stamp);
void rtc_timestamp_to_timestring(timestamp_t stamp, timestring_t string);
void rtc_timestring_to_timestamp(timestring_t string, timestamp_t stamp);

void rtc_periodic_update_handler();


