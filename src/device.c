#include "device.h"
#include <nrf.h>

uint32_t get_device_id() {
#ifdef TESTING
  return 0xDEADBEEF; // arbitrary device id for tests
#else
  return NRF_FICR->DEVICEID[1];
#endif
}

