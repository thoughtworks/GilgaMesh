#include "device.h"
#include "cmocka_includes.h"

uint32_t get_raw_device_id(void) {
  return 0x1234;
}

char* get_short_hex_device_id(uint32_t deviceId, char* result) {
  return "Mesh-1234";
}

