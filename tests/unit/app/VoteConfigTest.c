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

  voteConfiguration expectedConfig = { 17, 0 };
  expect_memory(update_data_in_storage, data, &expectedConfig, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, dataLength, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, recordKey, STORAGE_VOTE_CONFIG_RECORD_KEY);

  update_voting_group(group);
  assert_true(displayed_group_value_feedback);
}

static void VoteConfig_updates_voting_value_and_displays_feedback() {
  uint8_t value = 23;

  voteConfiguration expectedConfig = { 0, 23 };
  expect_memory(update_data_in_storage, data, &expectedConfig, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, dataLength, sizeof(voteConfiguration));
  expect_value(update_data_in_storage, recordKey, STORAGE_VOTE_CONFIG_RECORD_KEY);

  update_voting_value(value);
  assert_true(displayed_group_value_feedback);
}

int RunVoteConfigTest(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup(VoteConfig_updates_voting_group_and_displays_feedback, test_setup),
      cmocka_unit_test_setup(VoteConfig_updates_voting_value_and_displays_feedback, test_setup),
  };

  return cmocka_run_group_tests_name("VoteConfigTest", tests, NULL, NULL);
}

int main() {
  return RunVoteConfigTest();
}

