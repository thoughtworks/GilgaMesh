#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <fstorage.h>

void storage_initialize();
void* get_data_from_storage(uint16_t fileId, uint16_t recordKey);
void set_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey);
void update_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey);
void delete_data_from_storage(uint16_t fileId, uint16_t recordKey);
uint16_t get_record_count_in_storage(uint16_t fileId);
