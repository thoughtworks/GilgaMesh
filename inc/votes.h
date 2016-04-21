#include <stdint.h>
#include <stdbool.h>

#define MAX_VOTE_COUNT                  1000

uint16_t voterIds[MAX_VOTE_COUNT];

bool save_vote(uint16_t voterId);
uint16_t get_vote_count(void);

