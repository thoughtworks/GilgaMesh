#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "message_types/message.h"

#define VOTE_CONFIG_STORAGE_FILE_ID     0x0001
#define VOTE_CONFIG_STORAGE_RECORD_KEY  0x0001

typedef struct {
  uint8_t group;
  uint8_t value;
}__attribute__ ((packed)) voteConfiguration;

void update_voting_group(uint8_t newGroup);
void update_voting_value(uint8_t newValue);
void get_vote_configuration(voteConfiguration *result);
void print_current_vote_config(void);