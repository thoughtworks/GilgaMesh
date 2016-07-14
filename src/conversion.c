#include <conversion.h>
#include <stdio.h>

char* int_to_string(uint16_t i)
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 1];
  char *p = buf + INT_DIGITS;/* points to terminating '\0' */
  do {
    *--p = '0' + (i % 10);
    i /= 10;
  } while (i != 0);
  return p;
}


void set_node_name_from_device_id(uint32_t deviceId, char *nodeName)
{
  char hex_chars[] = "0123456789ABCDEF";
  char buf[NODE_NAME_SIZE] = "VB-0000";
  uint32_t deviceIdCopy = deviceId;

  for (uint8_t i = 2; i <= 5; i++){
    buf[NODE_NAME_SIZE - i] = hex_chars[deviceIdCopy & 0xf];
    deviceIdCopy >>= 4;
  }

  memcpy(nodeName, buf, NODE_NAME_SIZE);
}


void rtc_timestamp_to_timestring(timestamp_t stamp, timestring_t string) {
  for(int i = 0 ; i < TIMESTAMP_SIZE ; i++) {
    sprintf(string + 2 * i, "%02X", stamp[i]);
  }
  string[TIMESTRING_SIZE_INCLUDING_NULL - 1] = 0; // terminate with null
}

uint8_t hex_digit_to_uint8(char digit) {
  int retval = 0;

  for (int i = 0 ; i < 16 ; i++) {
    if( HEX_DIGITS[i] == digit ) {
      retval = i;
    }
  }

  return retval;
}

void rtc_timestring_to_timestamp(timestring_t string, timestamp_t stamp) {
  for(int i = 0 ; i < TIMESTAMP_SIZE ; i++) { // ignore the last (null) character
    stamp[i] = hex_digit_to_uint8(string[2 * i]) << 4; // tens
    stamp[i] += hex_digit_to_uint8(string[2 * i + 1]); // add units
  }
}

bool rtc_is_equal_timestamp(timestamp_t a, timestamp_t b)
{
  bool retval = true;

  for(int i = 0 ; i < 8 ; i++) {
    retval &= (a[i] == b[i]);
  }

  return retval;
}