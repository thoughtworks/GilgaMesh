#include "app/votes.h"

#include <sdk_common.h>
#include <stdlib.h>

#include "app/feedback.h"
#include "app/storage.h"

static userVote buffer = { 0 };
static userVote bufferForStorage __attribute__((aligned(4))) = { 0 };

static bool local_vote_buffer_is_empty() {
  return buffer.voterId == 0 && buffer.hitCount == 0;
}

bool add_buffer_contents_to_storage() {
  if (local_vote_buffer_is_empty()) return true;

  if (get_vote_count() > MAX_VOTE_COUNT) {
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

uint16_t get_vote_count() {
  uint16_t storedVotes = get_record_count_in_storage(VOTES_STORAGE_FILE_ID);
  return (uint16_t) (storedVotes + (local_vote_buffer_is_empty() ? 0 : 1));
}

bool vote_storage_is_full() {
#ifndef IS_PROD_BOARD
  return false;
#endif
  return get_vote_count() > MAX_VOTE_COUNT;
}