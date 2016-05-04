#include "votes.h"

#include "cmocka_includes.h"

static userVote vote;

static void test_user_votes_initialize_clears_vote_list() {
  user_votes_initialize();
  save_vote(123);
  save_vote(456);
  save_vote(789);

  user_votes_initialize();
  assert_int_equal(0, get_vote_count());
  assert_null(get_first_vote());
}

static void test_save_vote_increments_vote_count() {
  user_votes_initialize();
  assert_int_equal(0, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());

  save_vote(456);
  assert_int_equal(2, get_vote_count());

  save_vote(789);
  assert_int_equal(3, get_vote_count());
}

static void test_save_vote_does_not_increment_vote_count_when_voter_matches_last_voter() {
  user_votes_initialize();
  assert_int_equal(0, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());

  save_vote(123);
  assert_int_equal(1, get_vote_count());
}

static void test_save_vote_increments_hit_count_when_voter_matches_last_voter() {
  user_votes_initialize();
  save_vote(123);
  save_vote(123);
  save_vote(123);

  clear_vote_buffer(NULL, 0);
  userVote *vote = get_first_vote();
  assert_int_equal(3, vote->hitCount);
}

static void test_save_vote_fails_when_vote_list_is_full() {
  user_votes_initialize();
  for (int i = 1; i <= MAX_VOTE_COUNT; i++) {
    assert_true(save_vote(i));
  }
  assert_true(save_vote(8888)); //one extra, due to buffer
  assert_false(save_vote(9999));
}


int RunVotesTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(test_user_votes_initialize_clears_vote_list),
          cmocka_unit_test(test_save_vote_increments_vote_count),
          cmocka_unit_test(test_save_vote_does_not_increment_vote_count_when_voter_matches_last_voter),
          cmocka_unit_test(test_save_vote_increments_hit_count_when_voter_matches_last_voter),
          cmocka_unit_test(test_save_vote_fails_when_vote_list_is_full),
  };

  return cmocka_run_group_tests_name("VotesTest", tests, NULL, NULL);
}

int main() {
  RunVotesTest();
}
