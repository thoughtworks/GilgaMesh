#include <votes.h>
#include <string.h>
#include <linked_list.h>
#include <stdlib.h>
#include <sdk_common.h>
#include <timer.h>

static linkedList userVotes = { 0 };
static userVote buffer = { 0 };

void user_votes_initialize(void) {
  memset(&userVotes, 0, sizeof(userVotes));
  memset(&buffer, 0, sizeof(buffer));
  userVotes.maxSize = MAX_VOTE_COUNT;
}

bool buffer_is_empty() {
  return buffer.voterId == 0 && buffer.hitCount == 0;
}

bool add_buffer_contents_to_list() {
  if (buffer_is_empty()) return true;

  userVote *newVote = malloc(sizeof(userVote));
  memcpy(newVote, &buffer, sizeof(userVote));
  bool successful = add_item_to_list(&userVotes, (uint8_t *) newVote);
  if (successful) {
    memset(&buffer, 0, sizeof(buffer));
  } else {
    turn_on_buffer_clear_timeout();
    free(newVote);
  }
  return successful;
}

void clear_vote_buffer(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  add_buffer_contents_to_list();
}

bool save_vote(uint16_t voterId) {
  turn_off_buffer_clear_timeout();

  if (buffer.voterId == voterId) {
    buffer.hitCount++;
  } else {
    bool successful = add_buffer_contents_to_list();
    if (!successful) return false;

    buffer.voterId = voterId;
    buffer.hitCount = 1;
  }

  turn_on_buffer_clear_timeout();
  return true;
}

bool remove_vote(userVote *vote) {
  return remove_item_from_list(&userVotes, (uint8_t *) vote, sizeof(userVote));
}

userVote *get_first_vote() {
  return (userVote *) ((userVotes.count == 0) ? NULL : userVotes.first->item);
}

uint16_t get_vote_count() {
  return (uint16_t) (userVotes.count + (buffer_is_empty() ? 0 : 1));
}

