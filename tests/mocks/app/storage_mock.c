#include "app/storage.h"
#include "cmocka_includes.h"

void storage_initialize() { }

void* get_data_from_storage(uint16_t fileId, uint16_t recordKey) {
  return mock_type(void*);
}

void update_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey) {
  check_expected(data);
  check_expected(dataLength);
  check_expected(fileId);
  check_expected(recordKey);
}
