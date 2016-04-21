#include <votes.h>

bool save_vote(uint16_t voterId)
{
  bool successful = false;
  for (int i = 0; i < MAX_VOTE_COUNT; i++) {
    if (voterIds[i] == 0) {
      voterIds[i] = voterId;
      successful = true;
      break;
    }
  }
  return successful;
}

uint16_t get_vote_count()
{
  uint16_t voteCount = 0;
  for (int i = 0; i < MAX_VOTE_COUNT; i++) {
    if (voterIds[i] == 0) break;
    voteCount += 1;
  }
  return voteCount;
}