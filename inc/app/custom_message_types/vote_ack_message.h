#pragma once

#include "app/rtc.h"
#include "app/votes.h"


typedef struct {
  BleMessageHead head;
  uint32_t deviceId;
  userVoteAck voteAck;
}__attribute__ ((packed)) BleMessageVoteAckReq;

void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr);
void broadcast_vote_ack_from_cmd_line(char** parsedCommandArray, uint8_t numCommands);
MessagePropagationType receive_vote_acknowledgement(uint16_t connectionHandle, uint8_t *dataPacket);