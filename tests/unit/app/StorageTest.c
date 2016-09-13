#include "app/storage.h"
#include "cmocka_includes.h"
#include <stddef.h>
#include <fds.h>

extern bool fds_init_called;
extern bool fds_delete_called;
extern bool fds_write_called;
extern bool fds_open_called;
extern bool fds_close_called;

uint16_t recordKey;
uint16_t dataLength;
uint16_t data;

static int test_setup(void **state) {
  fds_init_called = false;
  fds_delete_called = false;
  fds_write_called = false;
  fds_open_called = false;
  fds_close_called = false;
  recordKey = 2;
  dataLength = 2;
  data = 3;
  return 0;
}

static void Storage_initialize_should_register_and_init() {
  expect_any(fds_register, cb);
  storage_initialize();
  assert_true(fds_init_called);
}

static void Storage_update_should_delete_existing_record_if_data_already_present() {
  will_return(fds_record_find, FDS_SUCCESS);

  update_data_in_storage(&data, dataLength, recordKey);
  assert_true(fds_delete_called);
  assert_true(fds_write_called);
}

static void Storage_update_should_not_delete_any_record_if_data_not_present() {
  will_return(fds_record_find, FDS_ERR_NOT_FOUND);

  update_data_in_storage(&data, dataLength, recordKey);
  assert_false(fds_delete_called);
  assert_true(fds_write_called);
}

static void Storage_get_data_finds_opens_closes_and_returns_data() {
  will_return(fds_record_find, FDS_SUCCESS);
  will_return(fds_record_open, FDS_SUCCESS);

  expect_any(fds_record_close, p_desc);
  assert_string_equal(get_data_from_storage(recordKey), "foo");
}

static void Storage_get_data_should_not_open_or_close_or_return_value_when_record_not_found() {
  will_return(fds_record_find, FDS_ERR_NOT_FOUND);

  assert_null(get_data_from_storage(recordKey));
  assert_false(fds_open_called);
  assert_false(fds_close_called);
}

static void Storage_get_data_should_not_close_or_return_value_when_record_not_opened() {
  will_return(fds_record_find, FDS_SUCCESS);
  will_return(fds_record_open, FDS_ERR_NOT_FOUND);

  assert_null(get_data_from_storage(recordKey));
  assert_false(fds_close_called);
}

int RunStorageTest(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup(Storage_initialize_should_register_and_init, test_setup),
      cmocka_unit_test_setup(Storage_update_should_delete_existing_record_if_data_already_present, test_setup),
      cmocka_unit_test_setup(Storage_update_should_not_delete_any_record_if_data_not_present, test_setup),
      cmocka_unit_test_setup(Storage_get_data_finds_opens_closes_and_returns_data, test_setup),
      cmocka_unit_test_setup(Storage_get_data_should_not_open_or_close_or_return_value_when_record_not_found, test_setup),
      cmocka_unit_test_setup(Storage_get_data_should_not_close_or_return_value_when_record_not_opened, test_setup),
  };

  return cmocka_run_group_tests_name("StorageTest", tests, NULL, NULL);
}

int main() {
  return RunStorageTest();
}


