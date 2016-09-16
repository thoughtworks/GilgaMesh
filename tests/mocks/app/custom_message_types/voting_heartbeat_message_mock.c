#include "app/custom_message_types/voting_heartbeat_message.h"
#include "cmocka_includes.h"

void voting_heartbeat_initialize(void) { }
void send_voting_heartbeat_message() { }
MessagePropagationType receive_voting_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  return PropagateToCentral;
}