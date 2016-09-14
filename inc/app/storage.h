#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <fstorage.h>

#define STORAGE_VOTE_CONFIG_RECORD_KEY  0x0001
#define STORAGE_VOTE_RECORD_KEY         0x0002

void storage_initialize();
void* get_data_from_storage(uint16_t recordKey);
void update_data_in_storage(void *data, uint16_t dataLength, uint16_t recordKey);
