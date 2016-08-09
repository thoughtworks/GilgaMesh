#include "message_types/heartbeat_message.h"

#include <cmocka_includes.h>

void send_heartbeat_message(void *data, uint16_t dataLength){ }

MessagePropagationType receive_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket){
  check_expected(connectionHandle);
  check_expected_ptr(dataPacket);
  return DoNotPropagate;
}
