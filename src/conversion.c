#include <conversion.h>

uint8_t hex_digit_to_uint8(char digit) {
  int retval = 0;

  for (int i = 0 ; i < 16 ; i++) {
    if( HEX_DIGITS[i] == digit ) {
      retval = i;
    }
  }

  return (uint8_t) retval;
}

void set_node_name_from_device_id(uint32_t deviceId, char *nodeName) {
  char hex_chars[] = "0123456789ABCDEF";
  char buf[NODE_NAME_SIZE] = "VB-0000";
  uint32_t deviceIdCopy = deviceId;

  for (uint8_t i = 2; i <= 5; i++){
    buf[NODE_NAME_SIZE - i] = hex_chars[deviceIdCopy & 0xf];
    deviceIdCopy >>= 4;
  }

  memcpy(nodeName, buf, NODE_NAME_SIZE);
}