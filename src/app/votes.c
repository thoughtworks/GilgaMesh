#include "app/votes.h"

#include <sdk_common.h>
#include <stdlib.h>

#include "app/feedback.h"
#include "app/storage.h"
#include "system/timer.h"
#include "system/util.h"
#include "command.h"

#define CLEAR_VOTE_BUFFER_DURATION_IN_MS 3000

static userVote buffer = { 0 };
static userVote bufferForStorage __attribute__((aligned(4))) = { 0 };

SYS_TIMER_DEF(clearBufferTimer);

bool votes_initialize() {
  mesh_add_terminal_command("vote", "Register a vote", save_vote_from_command_line);

  if (!execute_succeeds(create_single_shot_timer(&clearBufferTimer))) return false;
  return true;
}

void vote_six_times() {
  for(int i = 1; i <= 6; i++) {
    save_vote(i);
  }
}

static bool save_and_clear_vote_buffer_content();

storageOperationResult save_buffer_content_to_storage();

static void start_clear_buffer_timeout() {
  start_timer(&clearBufferTimer, CLEAR_VOTE_BUFFER_DURATION_IN_MS, save_and_clear_vote_buffer_content);
}

static void stop_clear_buffer_timeout() {
  stop_timer(&clearBufferTimer);
}

static bool local_vote_buffer_is_empty() {
  return buffer.voterId == 0 && buffer.hitCount == 0;
}

static bool save_and_clear_vote_buffer_content() {
  if (local_vote_buffer_is_empty()) return true;

  if (vote_storage_is_full()) {
    start_clear_buffer_timeout();
    return false;
  }

  if(save_buffer_content_to_storage() != SUCCESS) {
    start_clear_buffer_timeout();
    return false;
  }

  memset(&buffer, 0, sizeof(buffer));
  return true;
}

storageOperationResult save_buffer_content_to_storage() {
  memcpy(&bufferForStorage, &buffer, sizeof(buffer));
  return set_data_in_storage(&bufferForStorage, sizeof(bufferForStorage), VOTES_STORAGE_FILE_ID, bufferForStorage.voterId);
}

void save_vote(uint16_t voterId) {
  stop_clear_buffer_timeout();

  if (buffer.voterId == voterId) {
    buffer.hitCount++;
    display_vote_hit_recorded_feedback();
  } else {
    if (!save_and_clear_vote_buffer_content()) {
      display_failure_feedback();
      return;
    }

    if (!get_vote_configuration(&buffer.config)) {
      display_failure_feedback();
      return;
    }

    buffer.voterId = voterId;
    buffer.hitCount = 1;

    display_vote_recorded_feedback();
  }
  rtc_get_timestamp(buffer.timeOfLastVote);

  start_clear_buffer_timeout();
}

void save_vote_from_command_line(char** parsedCommandArray, uint8_t numCommands) {
  SYS_UNUSED_PARAMETER(numCommands);
  uint16_t voterId = (uint16_t) atoi(parsedCommandArray[1]);
  save_vote(voterId);
}

uint16_t get_vote_count() {
  uint16_t storedVotes = get_record_count_in_storage(VOTES_STORAGE_FILE_ID);
  return (uint16_t) (storedVotes + (local_vote_buffer_is_empty() ? 0 : 1));
}

bool vote_storage_is_full() {
  return get_vote_count() > MAX_VOTE_COUNT;
}

void clear_all_votes() {
  delete_file_from_storage(VOTES_STORAGE_FILE_ID);
  display_successful_update_feedback();

  if(!local_vote_buffer_is_empty()) {
    stop_clear_buffer_timeout();
    memset(&buffer, 0, sizeof(buffer));
    start_clear_buffer_timeout();
  }
}