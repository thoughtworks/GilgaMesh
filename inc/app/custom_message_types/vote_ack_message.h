#pragma once

#include "app/votes.h"


typedef struct {
  BleMessageType messageType;
  uint32_t deviceId;
  userVoteAck voteAck;
}__attribute__ ((packed)) BleMessageVoteAckReq;

bool vote_ack_message_initialize(void);
void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr);
void broadcast_vote_ack_from_cmd_line(char** parsedCommandArray, uint8_t numCommands);
MessagePropagationType receive_vote_acknowledgement(uint16_t connectionHandle, uint8_t *dataPacket);