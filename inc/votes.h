#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_VOTE_COUNT                  200

typedef struct {
  uint16_t voterId;
  uint16_t hitCount; // does this need to be uint16_t? also what happens with overflow? we should handle that
}__attribute__ ((packed)) userVote;

#define VOTE_STORAGE_SIZE               sizeof(userVote) * MAX_VOTE_COUNT

void user_votes_initialize(void);
bool save_vote(uint16_t voterId);
bool remove_vote(userVote *vote);
void clear_vote_buffer(void *data, uint16_t dataLength);
userVote *get_first_vote();
uint16_t get_vote_count(void);
