#include "app/vote_config.h"
#include "cmocka_includes.h"

bool get_vote_configuration(voteConfiguration *result) {
  check_expected(result);
  return true;
}

void print_current_vote_config(void) { }

void update_voting_group(uint8_t newGroup) {
  check_expected(newGroup);
}

void update_voting_value(uint8_t newValue) {
  check_expected(newValue);
}

bool vote_config_is_set() {
  return true;
}