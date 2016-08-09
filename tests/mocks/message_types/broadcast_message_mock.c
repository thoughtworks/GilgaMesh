#include "message_types/broadcast_message.h"

#include "cmocka_includes.h"

void send_broadcast_message(char **parsedCommandArray) {
  check_expected_ptr(parsedCommandArray);
}

MessagePropagationType receive_broadcast_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  check_expected(connectionHandle);
  check_expected_ptr(dataPacket);
  return DoNotPropagate;
}