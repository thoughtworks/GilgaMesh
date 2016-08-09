#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "message_types/message.h"

typedef struct {
  uint8_t group;
  uint8_t value;
}__attribute__ ((packed)) voteConfiguration;

voteConfiguration *get_vote_configuration();
void set_vote_configuration(uint8_t *data);
void update_voting_group(uint8_t newGroup);
void update_voting_value(uint8_t newValue);
void broadcast_group_value_update(char* nodeIdStr, char* newFieldStr, bool isGroup);
MessagePropagationType receive_group_value_update(uint16_t connectionHandle, uint8_t *dataPacket);
