#pragma once

#include "app/votes.h"

#define VOTE_MESSAGE_FREQUENCY_IN_MS 5000

typedef struct {
  uint8_t messageType;
  uint32_t deviceId;
  userVote vote;
}__attribute__ ((packed)) BleMessageVoteReq;

void vote_message_initialize(void);
void send_vote_message(void);
MessagePropagationType receive_vote_message(uint16_t connectionHandle, uint8_t *dataPacket);
