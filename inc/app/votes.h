#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <rtc.h>
#include "vote_config.h"

#define MAX_VOTE_COUNT                    500 // if you change this, you must also change PSTORAGE_NUM_OF_PAGES

typedef struct {
  uint16_t voterId;
  uint16_t hitCount; // what happens with overflow? we should handle that
  timestamp_t timeOfLastVote;
  voteConfiguration config;
}__attribute__ ((packed)) userVote;

typedef struct {
  uint16_t count;
  uint16_t startIndex;
}__attribute__ ((packed)) voteMetadata;

typedef struct {
  uint16_t voterId;
  timestamp_t timestamp;
}__attribute__ ((packed)) userVoteAck;


void user_votes_initialize(void);
void save_vote(uint16_t voterId);
void clear_vote_buffer(void *data, uint16_t dataLength);
void get_first_vote(void *data, uint16_t dataLength);
void process_vote_from_storage(userVote *vote);
uint16_t get_vote_count(void);
bool vote_storage_is_full(void);
void set_vote_metadata(voteMetadata *metadata);
void broadcast_vote(void *data, uint16_t dataLength);
void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr);
MessagePropagationType receive_vote(uint16_t connectionHandle, uint8_t *dataPacket);
MessagePropagationType receive_vote_acknowledgement(uint16_t connectionHandle, uint8_t *dataPacket);
