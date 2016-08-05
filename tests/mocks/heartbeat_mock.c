#include "heartbeat.h"

#include <cmocka_includes.h>

void broadcast_heartbeat(void *data, uint16_t dataLength){ }

MessagePropagationType receive_heartbeat(uint16_t connectionHandle, uint8_t *dataPacket){
  check_expected(connectionHandle);
  check_expected_ptr(dataPacket);
  return DoNotPropagate;
}
