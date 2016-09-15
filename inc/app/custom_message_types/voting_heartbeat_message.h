#pragma once

#include "app/nfc.h"
#include "app/vote_config.h"
#include "message_types/message.h"

typedef struct {
  BleMessageHead head;
  uint32_t deviceId;
  voteConfiguration voteConfig;
  uint16_t voteCount;
  nfcStatus nfcStatus;
}__attribute__ ((packed)) BleMessageVotingHeartbeatReq;


void voting_heartbeat_initialize(void);
void send_voting_heartbeat_message();
MessagePropagationType receive_voting_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket);
