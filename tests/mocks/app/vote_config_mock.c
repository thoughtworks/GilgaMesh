#include "app/vote_config.h"
#include "cmocka_includes.h"

voteConfiguration *get_vote_configuration() {
  return mock_ptr_type(voteConfiguration *);
}

void set_vote_configuration(uint8_t *data) { }

void update_voting_group(uint8_t newGroup) {
  check_expected(newGroup);
}

void update_voting_value(uint8_t newValue) {
  check_expected(newValue);
}