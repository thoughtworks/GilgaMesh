#include "app/vote_config.h"
#include "cmocka_includes.h"
#include "app/storage.h"

extern bool displayed_group_value_feedback;

static int test_setup(void **state) {
  displayed_group_value_feedback = false;
  return 0;
}

static void VoteConfig_updates_voting_group_and_displays_feedback() {
  uint8_t group = 17;
  will_return(get_data_from_storage, NULL);

  voteConfiguration expectedConfig = { 17, 0 };
  expect_memory(update_data_in_storage, data, &expectedConfig, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, dataLength, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, fileId, VOTE_CONFIG_STORAGE_FILE_ID);
  expect_value(update_data_in_storage, recordKey, VOTE_CONFIG_STORAGE_RECORD_KEY);

  update_voting_group(group);
  assert_true(displayed_group_value_feedback);
}

static void VoteConfig_updates_voting_value_and_displays_feedback() {
  uint8_t value = 23;
  will_return(get_data_from_storage, NULL);

  voteConfiguration expectedConfig = { 0, 23 };
  expect_memory(update_data_in_storage, data, &expectedConfig, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, dataLength, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, fileId, VOTE_CONFIG_STORAGE_FILE_ID);
  expect_value(update_data_in_storage, recordKey, VOTE_CONFIG_STORAGE_RECORD_KEY);

  update_voting_value(value);
  assert_true(displayed_group_value_feedback);
}

static void VoteConfig_update_result_with_null_vote_config_when_no_config_exists() {
  will_return(get_data_from_storage, NULL);
  voteConfiguration result;

  get_vote_configuration(&result);
  assert_int_equal(result.group, 0);
  assert_int_equal(result.value, 0);
}

static void VoteConfig_update_result_with_stored_config_if_it_exists() {
  voteConfiguration storedConfig = {2, 3};
  will_return(get_data_from_storage, &storedConfig);
  voteConfiguration result;

  get_vote_configuration(&result);
  assert_int_equal(result.group, 2);
  assert_int_equal(result.value, 3);
}

int RunVoteConfigTest(void) {
  const struct CMUnitTest tests[] = {
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

