#include "app/vote_config.h"
#include "cmocka_includes.h"

extern bool displayed_group_value_feedback;

static int test_setup(void **state) {
  uint8_t defaultVoteConfig[2] = {0, 0};
  set_vote_configuration(defaultVoteConfig);
  displayed_group_value_feedback = false;
  return 0;
}

static void VoteConfig_updates_voting_group() {
  uint8_t group = 17;
  update_voting_group(group);

  assert_int_equal(group, get_vote_configuration()->group);
}

static void VoteConfig_updates_voting_value() {
  uint8_t value = 23;
  update_voting_value(value);

  assert_int_equal(value, get_vote_configuration()->value);
}

static void VoteConfig_displays_feedback_on_value_update() {
  uint8_t value = 23;
  update_voting_value(value);

  assert_true(displayed_group_value_feedback);
}

static void VoteConfig_displays_feedback_on_group_update() {
  uint8_t group = 23;
  update_voting_group(group);

  assert_true(displayed_group_value_feedback);
}

int RunVoteConfigTest(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup(VoteConfig_updates_voting_group, test_setup),
      cmocka_unit_test_setup(VoteConfig_updates_voting_value, test_setup),
      cmocka_unit_test_setup(VoteConfig_displays_feedback_on_value_update, test_setup),
      cmocka_unit_test_setup(VoteConfig_displays_feedback_on_group_update, test_setup),
  };

  return cmocka_run_group_tests_name("VoteConfigTest", tests, NULL, NULL);
}

int main() {
  return RunVoteConfigTest();
}

