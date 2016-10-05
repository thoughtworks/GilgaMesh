#pragma once

#include "app/nfc.h"
#include "app/vote_config.h"
#include "message_types/message.h"

#define VOTING_MESSAGE_HEARTBEAT_FREQUENCY_IN_MS 3000

typedef struct {
  BleMessageType messageType;
  uint32_t deviceId;
  voteConfiguration voteConfig;
  uint16_t voteCount;
  nfcStatus nfcStatus;
}__attribute__ ((packed)) BleMessageVotingHeartbeatReq;


bool voting_heartbeat_message_initialize(void);
void send_voting_heartbeat_message();
MessagePropagationType receive_voting_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket);
