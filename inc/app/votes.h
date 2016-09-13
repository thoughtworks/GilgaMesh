#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "app/rtc.h"
#include "vote_config.h"

#define VOTES_STORAGE_FILE_ID     0x0002
#define MAX_VOTE_COUNT            500 // if you change this, you must also change PSTORAGE_NUM_OF_PAGES

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
void save_vote_from_command_line(char** parsedCommandArray, uint8_t numCommands);
void clear_vote_buffer(void *data, uint16_t dataLength);
void process_vote_from_storage(userVote *vote);
uint16_t get_vote_count(void);
bool vote_storage_is_full(void);
void set_vote_metadata(voteMetadata *metadata);
