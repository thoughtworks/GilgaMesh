#include "app/votes.h"
#include "cmocka_includes.h"

bool votes_initialize(void) {
  return true;
}

void save_vote_from_command_line(char** parsedCommandArray, uint8_t numCommands) { }

void save_vote(uint16_t voterId) {
  check_expected(voterId);
}

uint16_t get_vote_count(void) {
  return mock_type(uint16_t);
}

bool max_votes_recorded(void) {
  return mock_type(bool);
}