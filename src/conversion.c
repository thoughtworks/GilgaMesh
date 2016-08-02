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