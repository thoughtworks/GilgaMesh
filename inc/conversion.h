#pragma once

#include <stdint.h>
#include <string.h>
#include <message.h>
#include <rtc.h>
#include <stdbool.h>

#define INT_DIGITS 19/* enough for 64 bit integer */

static const char HEX_DIGITS[16] = "0123456789ABCDEF";

char* int_to_string(uint16_t i);
void set_node_name_from_device_id(uint32_t deviceId, char *nodeName);

uint8_t hex_digit_to_uint8(char digit);
void rtc_timestamp_to_timestring(timestamp_t stamp, timestring_t string);
void rtc_timestring_to_timestamp(timestring_t string, timestamp_t stamp);
bool rtc_is_equal_timestamp(timestamp_t a, timestamp_t b);