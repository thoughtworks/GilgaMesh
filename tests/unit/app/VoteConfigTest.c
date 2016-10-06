#include "app/vote_config.h"
#include "cmocka_includes.h"
#include "app/storage.h"

extern bool displayed_successful_update_feedback;
extern void* mockStoredData;
static uint8_t group = 17;
static uint8_t value = 23;

static int test_setup(void **state) {
  displayed_successful_update_feedback = false;
  return 0;
}

static void VoteConfig_initialize_creates_group_and_value_commands() {
  expect_string(mesh_add_terminal_command, commandName, "group");
  expect_string(mesh_add_terminal_command, commandName, "value");

  vote_config_initialize();
}

static void VoteConfig_updates_voting_group_and_displays_feedback() {
  will_return(update_data_in_storage, SUCCESS);

  expect_memory(update_data_in_storage, data, &group, sizeof(group));
  expect_value(update_data_in_storage, dataLength, sizeof(group));
  expect_value(update_data_in_storage, fileId, VOTE_CONFIG_STORAGE_FILE_ID);
  expect_value(update_data_in_storage, recordKey, VOTE_CONFIG_GROUP_STORAGE_RECORD_KEY);

  update_voting_group(group);
  assert_true(displayed_successful_update_feedback);
}

static void VoteConfig_updates_voting_value_and_displays_feedback() {
  will_return(update_data_in_storage, SUCCESS);

  expect_memory(update_data_in_storage, data, &value, sizeof(value));
  expect_value(update_data_in_storage, dataLength, sizeof(value));
  expect_value(update_data_in_storage, fileId, VOTE_CONFIG_STORAGE_FILE_ID);
  expect_value(update_data_in_storage, recordKey, VOTE_CONFIG_VALUE_STORAGE_RECORD_KEY);

  update_voting_value(value);
  assert_true(displayed_successful_update_feedback);
}

static void VoteConfig_update_result_with_null_vote_config_when_no_config_exists() {
  will_return_always(get_data_from_storage, NOT_FOUND);

  voteConfiguration result;
  get_vote_configuration(&result);

  assert_int_equal(result.group, INVALID_VOTE_CONFIG);
  assert_int_equal(result.value, INVALID_VOTE_CONFIG);
}

static void VoteConfig_update_result_with_stored_config_if_it_exists() {
  mockStoredData = &group;
  will_return_always(get_data_from_storage, SUCCESS);

  voteConfiguration result;
  get_vote_configuration(&result);

  assert_int_equal(result.group, group);
  assert_int_equal(result.value, group);
}

int RunVoteConfigTest(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup(VoteConfig_initialize_creates_group_and_value_commands, test_setup),
      cmocka_unit_test_setup(VoteConfig_updates_voting_group_and_displays_feedback, test_setup),
      cmocka_unit_test_setup(VoteConfig_updates_voting_value_and_displays_feedback, test_setup),
      cmocka_unit_test_setup(VoteConfig_update_result_with_null_vote_config_when_no_config_exists, test_setup),
      cmocka_unit_test_setup(VoteConfig_update_result_with_stored_config_if_it_exists, test_setup),
  };

  return cmocka_run_group_tests_name("VoteConfigTest", tests, NULL, NULL);
}

int main() {
  return RunVoteConfigTest();
}

