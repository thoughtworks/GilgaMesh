#include <votes.h>
#include <string.h>

static userVote userVotes[MAX_VOTE_COUNT] = { 0 };
static uint16_t voteCount = 0;

userVote *last_vote() {
  return &userVotes[voteCount - 1];
}

bool save_vote(uint16_t voterId) {
  if (voteCount == MAX_VOTE_COUNT) return false;

  if (last_vote()->voterId == voterId) {
    last_vote()->hitCount++;
  } else {
    userVote newVote = { voterId, 1 };
    userVotes[voteCount] = newVote;
    voteCount++;
  }
  return true;
}

bool collect_vote(userVote *vote) {
  if (voteCount == 0) return false;

  *vote = *last_vote();
  memset(last_vote(), 0, sizeof(userVote));
  voteCount--;
  return true;
}

uint16_t get_vote_count() {
  return voteCount;
}

void reset_votes(void) {
  voteCount = 0;
  memset(userVotes, 0, sizeof(userVotes));
}
