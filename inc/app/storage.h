#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <fstorage.h>

typedef enum {
  SUCCESS,
  FAILURE,
  NOT_FOUND
} storageOperationResult;

void storage_initialize();
storageOperationResult get_data_from_storage(uint16_t fileId, uint16_t recordKey, void* storedData, uint16_t dataLength);
storageOperationResult set_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey);
storageOperationResult update_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey);
storageOperationResult delete_data_from_storage(uint16_t fileId, uint16_t recordKey);
uint16_t get_record_count_in_storage(uint16_t fileId);
void delete_file_from_storage(uint16_t fileId);
void check_fds_stats();
void whats_in_storage_right_now();
