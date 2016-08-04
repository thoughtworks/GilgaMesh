#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <pstorage.h>

#include "votes.h"


void storage_initialize();
void set_vote_in_storage(userVote *vote, uint16_t index);
void get_vote_from_storage(uint16_t index);
void set_vote_metadata_in_storage(voteMetadata *metadata);
void save_vote_configuration(void);
void load_version_information(void);
