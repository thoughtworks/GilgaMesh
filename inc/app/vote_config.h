#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "message_types/message.h"

#define VOTE_CONFIG_STORAGE_FILE_ID           0x0001
#define VOTE_CONFIG_GROUP_STORAGE_RECORD_KEY  0x0001
#define VOTE_CONFIG_VALUE_STORAGE_RECORD_KEY  0x0002

#define INVALID_VOTE_CONFIG 255

typedef struct {
  uint8_t group;
  uint8_t value;
}__attribute__ ((packed)) voteConfiguration;

bool vote_config_initialize(void);
void update_voting_group(uint8_t newGroup);
void update_voting_value(uint8_t newValue);
bool get_vote_configuration(voteConfiguration *result);
bool vote_config_is_set(void);
void set_vote_config_group(char **parsedCommandArray, uint8_t numCommands);
void set_vote_config_value(char **parsedCommandArray, uint8_t numCommands);

