#include "message_types/heartbeat_message.h"

#include <cmocka_includes.h>

bool heartbeat_message_initialize() {
  return true;
}

void send_heartbeat_message(void) { }

MessagePropagationType receive_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket){
  check_expected(connectionHandle);
  check_expected_ptr(dataPacket);
  return DoNotPropagate;
}
