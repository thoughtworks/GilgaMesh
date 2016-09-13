#include "app/votes.h"

#include <limits.h>
#include <sdk_common.h>
#include <stdlib.h>

#include "app/feedback.h"
#include "app/storage.h"

static voteMetadata votes = { 0, 0 };
static userVote buffer = { 0 };
static userVote bufferForStorage __attribute__((aligned(4))) = { 0 };
static userVoteAck voteAckBuffer = { 0, 0 };

static bool local_vote_buffer_is_empty() {
  return buffer.voterId == 0 && buffer.hitCount == 0;
}

bool add_buffer_contents_to_storage() {
  if (local_vote_buffer_is_empty()) return true;

  if (votes.count >= MAX_VOTE_COUNT) {
    //turn_on_buffer_clear_timeout();
    return false;
  }

  memcpy(&bufferForStorage, &buffer, sizeof(buffer));
  set_data_in_storage(&bufferForStorage, sizeof(bufferForStorage), VOTES_STORAGE_FILE_ID, bufferForStorage.voterId);
  memset(&buffer, 0, sizeof(buffer));

  return true;
}

void clear_vote_buffer(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  add_buffer_contents_to_storage();
}

void save_vote(uint16_t voterId) {
  //turn_off_buffer_clear_timeout();

  if (buffer.voterId == voterId) {
    buffer.hitCount++;
  } else {
    bool successful = add_buffer_contents_to_storage();
    if (!successful) return;

    buffer.voterId = voterId;
    buffer.hitCount = 1;
    get_vote_configuration(&buffer.config);

    display_vote_recorded_feedback();
  }
  rtc_get_timestamp(buffer.timeOfLastVote);

  //turn_on_buffer_clear_timeout();
}

void save_vote_from_command_line(char** parsedCommandArray, uint8_t numCommands) {
  uint16_t voterId = (uint16_t) atoi(parsedCommandArray[1]);
  save_vote(voterId);
}

bool acknowledgement_buffer_matches_vote(userVote *vote) {
  if (voteAckBuffer.voterId != vote->voterId) return false;
  if (!rtc_is_equal_timestamp(voteAckBuffer.timestamp, vote->timeOfLastVote)) return false;

  return true;
}

void process_vote_from_storage(userVote *vote) {
  if (vote->voterId == USHRT_MAX) return; // we tried to access storage before the vote was saved. whoops.

    //turn_off_send_vote_timeout();
    memset(&voteAckBuffer, 0, sizeof(voteAckBuffer));
//    get_random_vote(NULL, 0);
    //turn_on_send_vote_timeout();
}

uint16_t get_vote_count() {
  uint16_t storedVotes = get_record_count_in_storage(VOTES_STORAGE_FILE_ID);
  return (uint16_t) (storedVotes + (local_vote_buffer_is_empty() ? 0 : 1));
}

bool vote_storage_is_full() {
#ifndef PROD_BOARD
  return false;
#endif
  return get_vote_count() == MAX_VOTE_COUNT + 1;
}

void user_votes_initialize(void) {
  memset(&votes, 0, sizeof(votes));
  memset(&buffer, 0, sizeof(buffer));
  memset(&voteAckBuffer, 0, sizeof(voteAckBuffer));
  //turn_on_send_vote_timeout();
}