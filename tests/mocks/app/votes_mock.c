#include "app/votes.h"
#include "cmocka_includes.h"

void votes_initialize(void) { }
void save_vote_from_command_line(char** parsedCommandArray, uint8_t numCommands) { }

void save_vote(uint16_t voterId) {
  check_expected(voterId);
}

uint16_t get_vote_count(void) {
  return mock_type(uint16_t);
}

bool vote_storage_is_full(void) {
  return mock_type(bool);
}