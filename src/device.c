#include "device.h"
#include <nrf.h>
#include <string.h>

static const char HEX_DIGITS[16] = "0123456789ABCDEF";

uint32_t get_raw_device_id() {
  return NRF_FICR->DEVICEID[1];
}

char* get_short_hex_device_id(uint32_t deviceId, char* result) {
  memcpy(result, HEX_DEVICE_ID_TEMPLATE, HEX_DEVICE_ID_LENGTH);
  uint32_t deviceIdCopy = deviceId;

  for (uint8_t i = 2; i <= 5; i++){
    result[HEX_DEVICE_ID_LENGTH - i] = HEX_DIGITS[deviceIdCopy & 0xf];
    deviceIdCopy >>= 4;
  }

  return result;
}