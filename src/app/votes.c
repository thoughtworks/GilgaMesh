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

SYS_TIMER_DEF(clearBufferTimer);

bool votes_initialize() {
  mesh_add_terminal_command("vote", "Register a vote", save_vote_from_command_line);

  if (!execute_succeeds(create_single_shot_timer(&clearBufferTimer))) return false;
  return true;
}

static void save_and_clear_vote_buffer_content();

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

static bool vote_storage_is_full() {
  return get_record_count_in_storage(VOTES_STORAGE_FILE_ID) >= MAX_VOTE_COUNT;
}

static void save_and_clear_vote_buffer_content() {
  if (local_vote_buffer_is_empty()) return;

  if (vote_storage_is_full() || (save_buffer_content_to_storage() == SUCCESS)) {
    memset(&buffer, 0, sizeof(buffer));
  } else {
    start_clear_buffer_timeout();
  }
}

storageOperationResult save_buffer_content_to_storage() {
  return set_data_in_storage(&buffer, sizeof(buffer), VOTES_STORAGE_FILE_ID, buffer.voterId);
}

static ret_code_t save_new_vote_to_buffer(uint16_t voterId) {
  if (!get_vote_configuration(&buffer.config)) {
    display_failure_feedback();
    return NRF_ERROR_INTERNAL;
  }
  buffer.voterId = voterId;
  buffer.hitCount = 1;
  rtc_get_timestamp(buffer.timeOfLastVote);
  return NRF_SUCCESS;
}

void save_vote(uint16_t voterId) {
  stop_clear_buffer_timeout();

  if (buffer.voterId == voterId) {
    buffer.hitCount++;
  }
  else {
    if (vote_storage_is_full()) {
      if (save_new_vote_to_buffer(voterId) == NRF_SUCCESS) {
        display_failure_feedback();
      }
    }
    else {
      if (local_vote_buffer_is_empty() || save_buffer_content_to_storage() == SUCCESS) {
        if (save_new_vote_to_buffer(voterId) == NRF_SUCCESS) {
          display_vote_recorded_feedback();
        }
      } else {
        display_failure_feedback();
      }
    }
  }

  start_clear_buffer_timeout();
}

void save_vote_from_command_line(char** parsedCommandArray, uint8_t numCommands) {
  SYS_UNUSED_PARAMETER(numCommands);
  uint16_t voterId = (uint16_t) atoi(parsedCommandArray[1]);
  save_vote(voterId);
}

uint16_t get_vote_count() {
  uint16_t storedVotes = get_record_count_in_storage(VOTES_STORAGE_FILE_ID);
  if (storedVotes == MAX_VOTE_COUNT) return storedVotes;
  return (uint16_t) (storedVotes + (local_vote_buffer_is_empty() ? 0 : 1));
}

bool max_votes_recorded() {
  return get_vote_count() >= MAX_VOTE_COUNT;
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