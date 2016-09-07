#include "app/vote_config.h"
#include "app/feedback.h"

static voteConfiguration voteConfig = {0, 0};

void update_voting_group(uint8_t newGroup) {
  if (newGroup != voteConfig.group) {
    voteConfig.group = newGroup;
    display_group_value_change_feedback();
//    save_vote_configuration();
  }
}

void update_voting_value(uint8_t newValue) {
  if (newValue != voteConfig.value) {
    voteConfig.value = newValue;
    display_group_value_change_feedback();
//    save_vote_configuration();
  }
}

voteConfiguration *get_vote_configuration() {
  return &voteConfig;
}

void set_vote_configuration(uint8_t *data) {
  update_voting_group(data[0]);
  update_voting_value(data[1]);
}
