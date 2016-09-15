#pragma once

#include "app/votes.h"

typedef struct {
  uint8_t messageType;
  uint32_t deviceId;
  userVote vote;
}__attribute__ ((packed)) BleMessageVoteReq;

void vote_message_initialize(void);
void broadcast_next_vote(void);
MessagePropagationType receive_vote(uint16_t connectionHandle, uint8_t *dataPacket);