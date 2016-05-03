#include "votes.h"

#include "cmocka_includes.h"

static userVote vote;

static void test_save_vote_stores_voter_id() {
  reset_votes();
  save_vote(1234);

  collect_vote(&vote);
  assert_int_equal(1234, vote.voterId);
}

static void test_save_vote_increments_the_vote_count() {
  reset_votes();
  assert_int_equal(0, get_vote_count());
  save_vote(1234);

  assert_int_equal(1, get_vote_count());
}

static void test_save_vote_increments_hit_count_when_voter_matches_last_voter() {
  reset_votes();
  save_vote(1234);
  save_vote(1234);
  save_vote(1234);

  collect_vote(&vote);
  assert_int_equal(3, vote.hitCount);
}

static void test_save_vote_does_not_increment_the_vote_count_when_voter_matches_last_voter() {
  reset_votes();
  assert_int_equal(0, get_vote_count());
  save_vote(1234);
  save_vote(1234);
  save_vote(1234);

  assert_int_equal(1, get_vote_count());
}

static void test_save_vote_returns_false_when_vote_storage_is_full() {
  reset_votes();
  for (int i = 1; i <= MAX_VOTE_COUNT; i++) {
    assert_true(save_vote(i));
  }

  assert_false(save_vote(9999));
}

static void test_collect_vote_decrements_the_vote_count() {
  reset_votes();
  save_vote(1234);
  assert_int_equal(1, get_vote_count());

  collect_vote(&vote);
  assert_int_equal(0, get_vote_count());
}

static void test_collect_vote_returns_true_when_votes_exist() {
  reset_votes();
  save_vote(1234);

  assert_true(collect_vote(&vote));
}

static void test_collect_vote_returns_false_when_votes_do_not_exist() {
  reset_votes();

  assert_false(collect_vote(&vote));
}

static void test_reset_votes_reinitializes_vote_count() {
  reset_votes();
  save_vote(123);
  save_vote(456);
  save_vote(789);
  assert_int_equal(3, get_vote_count());

  reset_votes();
  assert_int_equal(0, get_vote_count());
}

int RunVotesTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(test_save_vote_stores_voter_id),
          cmocka_unit_test(test_save_vote_increments_the_vote_count),
          cmocka_unit_test(test_save_vote_increments_hit_count_when_voter_matches_last_voter),
          cmocka_unit_test(test_save_vote_does_not_increment_the_vote_count_when_voter_matches_last_voter),
          cmocka_unit_test(test_save_vote_returns_false_when_vote_storage_is_full),
          cmocka_unit_test(test_collect_vote_decrements_the_vote_count),
          cmocka_unit_test(test_collect_vote_returns_true_when_votes_exist),
          cmocka_unit_test(test_collect_vote_returns_false_when_votes_do_not_exist),
          cmocka_unit_test(test_reset_votes_reinitializes_vote_count),
  };

  return cmocka_run_group_tests_name("VotesTest", tests, NULL, NULL);
}

int main() {
  RunVotesTest();
}
