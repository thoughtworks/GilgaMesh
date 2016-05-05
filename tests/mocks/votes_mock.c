#include "votes.h"

#include "cmocka_includes.h"

bool save_vote(uint16_t voterId)
{
  check_expected(voterId);
  return mock_type(bool);
}

userVote *get_first_vote() {
  return mock_ptr_type(userVote *);
}

uint16_t get_vote_count()
{
  return mock_type(uint16_t);
}

bool remove_vote(userVote *vote) {

}
