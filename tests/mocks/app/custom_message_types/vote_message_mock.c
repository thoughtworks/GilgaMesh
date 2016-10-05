#include "app/custom_message_types/vote_message.h"

#include <cmocka_includes.h>

bool did_broadcast_next_vote = false;

bool vote_message_initialize(void) {
  return true;
}

void send_vote_message(void) {
  did_broadcast_next_vote = true;
}
MessagePropagationType receive_vote_message(uint16_t connectionHandle, uint8_t *dataPacket) { }

