#include "app/vote_config.h"

#include <string.h>
#include "app/feedback.h"
#include "app/storage.h"
#include "system/log.h"

voteConfiguration tempVoteConfig __attribute__((aligned(4)));

void update_voting_group(uint8_t newGroup) {
  get_vote_configuration(&tempVoteConfig);
  tempVoteConfig.group = newGroup;
  MESH_LOG("Updated group to: %u", newGroup);
  display_group_value_change_feedback();
  update_data_in_storage(&tempVoteConfig, sizeof(voteConfiguration), VOTE_CONFIG_STORAGE_FILE_ID, VOTE_CONFIG_STORAGE_RECORD_KEY);
}

void update_voting_value(uint8_t newValue) {
  get_vote_configuration(&tempVoteConfig);
  tempVoteConfig.value = newValue;
  MESH_LOG("Updated value to: %u", newValue);
  display_group_value_change_feedback();
  update_data_in_storage(&tempVoteConfig, sizeof(voteConfiguration), VOTE_CONFIG_STORAGE_FILE_ID, VOTE_CONFIG_STORAGE_RECORD_KEY);
}

void get_vote_configuration(voteConfiguration *result) {
  void* savedVoteConfig = get_data_from_storage(VOTE_CONFIG_STORAGE_FILE_ID, VOTE_CONFIG_STORAGE_RECORD_KEY);
  if (savedVoteConfig == NULL) {
    result->group = 0;
    result->value = 0;
  } else {
    memcpy(result, savedVoteConfig, sizeof(voteConfiguration));
  }
}

void print_current_vote_config() {
  voteConfiguration voteConfig;
  get_vote_configuration(&voteConfig);
  MESH_LOG("   Group: %u, Value: %u\r\n", voteConfig.group, voteConfig.value);
}
