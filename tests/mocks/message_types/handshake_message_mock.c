#include "message_types/handshake_message.h"

#include "cmocka_includes.h"

MessagePropagationType receive_handshake_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  check_expected(connectionHandle);
  check_expected_ptr(dataPacket);
  return DoNotPropagate;
}

void send_handshake_message(connection *targetConnection) { }