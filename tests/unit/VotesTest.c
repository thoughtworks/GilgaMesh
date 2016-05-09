#include "votes.h"

#include "cmocka_includes.h"

static userVote vote;

static int setup_votes_test(void **state) {
  expect_any(pstorage_register, p_module_param);
  expect_value(pstorage_load, size, VOTE_STORAGE_SIZE);
  user_votes_initialize();
  return 0;
}

static void test_user_votes_initialize_loads_votes_from_storage() {
  save_vote(123);
  expect_any(pstorage_clear, size);
  save_vote(456);
  expect_any(pstorage_clear, size);
  save_vote(789);
  expect_any(pstorage_clear, size);
  clear_vote_buffer(NULL, 0);

  expect_any(pstorage_register, p_module_param);
  expect_any(pstorage_load, size);
  user_votes_initialize();
}

static void test_save_vote_increments_vote_count() {
  assert_int_equal(0, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());

  expect_value(pstorage_clear, size, VOTE_STORAGE_SIZE);
  save_vote(456);
  assert_int_equal(2, get_vote_count());

  expect_value(pstorage_clear, size, VOTE_STORAGE_SIZE);
  save_vote(789);
  assert_int_equal(3, get_vote_count());
}

static void test_save_vote_does_not_increment_vote_count_when_voter_matches_last_voter() {
  assert_int_equal(0, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());
}

static void test_save_vote_increments_hit_count_when_voter_matches_last_voter() {
  save_vote(123);
  save_vote(123);
  save_vote(123);

  expect_value(pstorage_clear, size, VOTE_STORAGE_SIZE);
  clear_vote_buffer(NULL, 0);
  userVote *vote = get_first_vote();
  assert_int_equal(3, vote->hitCount);
}

static void test_save_vote_fails_when_vote_list_is_full() {
  for (int i = 1; i <= MAX_VOTE_COUNT; i++) {
    assert_true(save_vote(i));
    expect_any(pstorage_clear, size);
  }
  assert_true(save_vote(8888)); //one extra, due to buffer

  assert_false(save_vote(9999));
}

static void test_remove_vote_puts_new_list_into_storage() {
  save_vote(123);
  expect_any(pstorage_clear, size);
  save_vote(456);
  expect_any(pstorage_clear, size);
  save_vote(789);

  userVote vote = { 123, 1 };
  expect_any(pstorage_clear, size);
  remove_vote(&vote);
}

int RunVotesTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(test_user_votes_initialize_loads_votes_from_storage, setup_votes_test),
          cmocka_unit_test_setup(test_save_vote_increments_vote_count, setup_votes_test),
          cmocka_unit_test_setup(test_save_vote_does_not_increment_vote_count_when_voter_matches_last_voter, setup_votes_test),
          cmocka_unit_test_setup(test_save_vote_increments_hit_count_when_voter_matches_last_voter, setup_votes_test),
          cmocka_unit_test_setup(test_save_vote_fails_when_vote_list_is_full, setup_votes_test),
          cmocka_unit_test_setup(test_remove_vote_puts_new_list_into_storage, setup_votes_test),
  };

  return cmocka_run_group_tests_name("VotesTest", tests, NULL, NULL);
}

int main() {
  RunVotesTest();
}
