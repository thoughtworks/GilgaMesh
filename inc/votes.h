#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_VOTE_COUNT                  1000

typedef struct {
  uint16_t voterId;
  uint16_t hitCount;
}__attribute__ ((packed)) userVote;


void user_votes_initialize(void);
bool save_vote(uint16_t voterId);
bool remove_vote(userVote *vote);
void clear_vote_buffer(void *data, uint16_t dataLength);
userVote *get_first_vote();
uint16_t get_vote_count(void);

