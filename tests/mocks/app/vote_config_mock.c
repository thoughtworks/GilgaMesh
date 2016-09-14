#include "app/vote_config.h"
#include "cmocka_includes.h"

void get_vote_configuration(voteConfiguration *result) { }

void print_current_vote_config(void) { }

void update_voting_group(uint8_t newGroup) {
  check_expected(newGroup);
}

void update_voting_value(uint8_t newValue) {
  check_expected(newValue);
}