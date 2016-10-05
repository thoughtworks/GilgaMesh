#include "app/storage.h"
#include "cmocka_includes.h"

void* mockStoredData;
bool delete_data_called;

void storage_initialize() { }

storageOperationResult get_data_from_storage(uint16_t fileId, uint16_t recordKey, void* storedData, uint16_t dataLength) {
  storageOperationResult result = mock_type(storageOperationResult);
  if (result == SUCCESS) {
    memcpy(storedData, mockStoredData, dataLength);
  }
  return result;
}

storageOperationResult delete_data_from_storage(uint16_t fileId, uint16_t recordKey) {
  delete_data_called = true;
  check_expected(fileId);
  check_expected(recordKey);
  return mock_type(storageOperationResult);
}

storageOperationResult update_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey) {
  check_expected(data);
  check_expected(dataLength);
  check_expected(fileId);
  check_expected(recordKey);
  return mock_type(storageOperationResult);
}
