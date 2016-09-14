#include "app/storage.h"
#include "cmocka_includes.h"

void storage_initialize() { }

void* get_data_from_storage(uint16_t recordKey) {
  return NULL;
}

void update_data_in_storage(void *data, uint16_t dataLength, uint16_t recordKey) {
  check_expected(data);
  check_expected(dataLength);
  check_expected(recordKey);
}
