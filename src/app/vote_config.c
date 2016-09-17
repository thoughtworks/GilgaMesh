#include "app/vote_config.h"

#include <stddef.h>
#include "app/feedback.h"
#include "app/storage.h"
#include "system/log.h"

uint8_t tempVal __attribute__((aligned(4)));

static void update_vote_config_field(const char* fieldName, uint8_t newVal, uint16_t recordKey) {
  tempVal = newVal;
  update_data_in_storage(&tempVal, sizeof(tempVal), VOTE_CONFIG_STORAGE_FILE_ID, recordKey);
  display_group_value_change_feedback();
  MESH_LOG("Updated vote config %s to: %u", fieldName, newVal);
}

static uint8_t get_stored_val(uint16_t recordKey) {
  uint8_t* storedVal = get_data_from_storage(VOTE_CONFIG_STORAGE_FILE_ID, recordKey);
  return (storedVal == NULL) ? (uint8_t) 0 : *storedVal;
}

void update_voting_group(uint8_t newGroup) {
  update_vote_config_field("group", newGroup, VOTE_CONFIG_GROUP_STORAGE_RECORD_KEY);
}

void update_voting_value(uint8_t newValue) {
  update_vote_config_field("value", newValue, VOTE_CONFIG_VALUE_STORAGE_RECORD_KEY);
}

void get_vote_configuration(voteConfiguration *result) {
  result->group = get_stored_val(VOTE_CONFIG_GROUP_STORAGE_RECORD_KEY);
  result->value = get_stored_val(VOTE_CONFIG_VALUE_STORAGE_RECORD_KEY);
}
