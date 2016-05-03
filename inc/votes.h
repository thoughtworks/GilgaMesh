#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_VOTE_COUNT                  1000

typedef struct {
  uint16_t voterId;
  uint16_t hitCount;
} userVote;

bool save_vote(uint16_t voterId);
bool collect_vote(userVote *vote);
uint16_t get_vote_count(void);
void reset_votes(void);

