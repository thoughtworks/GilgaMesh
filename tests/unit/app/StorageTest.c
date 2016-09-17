#include "app/storage.h"
#include "cmocka_includes.h"
#include "sdk_config.h"
#include <stddef.h>
#include <fds.h>

extern bool fds_init_called;
extern bool fds_delete_called;
extern bool fds_write_called;
extern bool fds_open_called;
extern bool fds_close_called;
extern bool fds_gc_called;
extern uint16_t words_used;

uint16_t fileId;
uint16_t recordKey;
uint16_t dataLength;
uint16_t data;

uint16_t total_flash_space = (FDS_VIRTUAL_PAGES - 1) * FDS_VIRTUAL_PAGE_SIZE;

static int test_setup(void **state) {
  fds_init_called = false;
  fds_delete_called = false;
  fds_write_called = false;
  fds_open_called = false;
  fds_close_called = false;
  fds_gc_called = false;
  words_used = 0;
  fileId = 1;
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

static void Storage_delete_should_delete_record_if_data_exists() {
  will_return(fds_record_find, FDS_SUCCESS);

  delete_data_from_storage(fileId, recordKey);
  assert_true(fds_delete_called);
}

static void Storage_delete_should_not_delete_record_if_data_not_found() {
  will_return(fds_record_find, FDS_ERR_NOT_FOUND);

  delete_data_from_storage(fileId, recordKey);
  assert_false(fds_delete_called);
}

static void Storage_update_should_delete_record_if_data_exists() {
  will_return(fds_record_find, FDS_SUCCESS);

  update_data_in_storage(&data, dataLength, fileId, recordKey);
  assert_true(fds_delete_called);
  assert_true(fds_write_called);
}

static void Storage_update_should_not_delete_record_if_data_not_found() {
  will_return(fds_record_find, FDS_ERR_NOT_FOUND);

  update_data_in_storage(&data, dataLength, fileId, recordKey);
  assert_false(fds_delete_called);
  assert_true(fds_write_called);
}

static void Storage_get_data_finds_opens_closes_and_returns_data() {
  will_return(fds_record_find, FDS_SUCCESS);
  will_return(fds_record_open, FDS_SUCCESS);

  expect_any(fds_record_close, p_desc);
  assert_string_equal(get_data_from_storage(fileId, recordKey), "foo");
}

static void Storage_get_data_should_not_open_or_close_or_return_value_when_record_not_found() {
  will_return(fds_record_find, FDS_ERR_NOT_FOUND);

  assert_null(get_data_from_storage(fileId, recordKey));
  assert_false(fds_open_called);
  assert_false(fds_close_called);
}

static void Storage_get_data_should_not_close_or_return_value_when_record_not_opened() {
  will_return(fds_record_find, FDS_SUCCESS);
  will_return(fds_record_open, FDS_ERR_NOT_FOUND);

  assert_null(get_data_from_storage(fileId, recordKey));
  assert_false(fds_close_called);
}

static void Storage_get_record_count_should_return_correct_number() {
  will_return(fds_record_find_in_file, FDS_SUCCESS);
  will_return(fds_record_find_in_file, FDS_SUCCESS);
  will_return(fds_record_find_in_file, FDS_SUCCESS);
  will_return(fds_record_find_in_file, FDS_ERR_NOT_FOUND);

  assert_int_equal(3, get_record_count_in_storage(fileId));
}

static void Storage_delete_should_garbage_collect_if_diff_btwn_total_and_used_words_is_gte_five_percent() {
  uint16_t ninety_five_percent_of_flash = total_flash_space * .95 + 1;
  will_return(fds_record_find, FDS_SUCCESS);
  words_used = ninety_five_percent_of_flash;

  delete_data_from_storage(fileId, recordKey);
  assert_true(fds_gc_called);
}

static void Storage_delete_should_not_garbage_collect_if_diff_btwn_total_and_used_words_is_lt_five_percent() {
  uint16_t ninety_four_percent_of_flash = total_flash_space * .94;
  will_return(fds_record_find, FDS_SUCCESS);
  words_used = ninety_four_percent_of_flash;

  delete_data_from_storage(fileId, recordKey);
  assert_false(fds_gc_called);
}

static void Storage_set_should_garbage_collect_if_diff_btwn_total_and_used_words_is_gte_five_percent() {
  uint16_t ninety_five_percent_of_flash = total_flash_space * .95 + 1;
  words_used = ninety_five_percent_of_flash;

  set_data_in_storage("foo", 4, fileId, recordKey);
  assert_true(fds_gc_called);
}

static void Storage_set_should_not_garbage_collect_if_diff_btwn_total_and_used_words_is_lt_five_percent() {
  uint16_t ninety_four_percent_of_flash = total_flash_space * .94;
  words_used = ninety_four_percent_of_flash;

  set_data_in_storage("foo", 4, fileId, recordKey);
  assert_false(fds_gc_called);
}

int RunStorageTest(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup(Storage_initialize_should_register_and_init, test_setup),
      cmocka_unit_test_setup(Storage_delete_should_delete_record_if_data_exists, test_setup),
      cmocka_unit_test_setup(Storage_delete_should_not_delete_record_if_data_not_found, test_setup),
      cmocka_unit_test_setup(Storage_update_should_delete_record_if_data_exists, test_setup),
      cmocka_unit_test_setup(Storage_update_should_not_delete_record_if_data_not_found, test_setup),
      cmocka_unit_test_setup(Storage_get_data_finds_opens_closes_and_returns_data, test_setup),
      cmocka_unit_test_setup(Storage_get_data_should_not_open_or_close_or_return_value_when_record_not_found, test_setup),
      cmocka_unit_test_setup(Storage_get_data_should_not_close_or_return_value_when_record_not_opened, test_setup),
      cmocka_unit_test_setup(Storage_get_record_count_should_return_correct_number, test_setup),
      cmocka_unit_test_setup(Storage_delete_should_garbage_collect_if_diff_btwn_total_and_used_words_is_gte_five_percent, test_setup),
      cmocka_unit_test_setup(Storage_delete_should_not_garbage_collect_if_diff_btwn_total_and_used_words_is_lt_five_percent, test_setup),
      cmocka_unit_test_setup(Storage_set_should_garbage_collect_if_diff_btwn_total_and_used_words_is_gte_five_percent, test_setup),
      cmocka_unit_test_setup(Storage_set_should_not_garbage_collect_if_diff_btwn_total_and_used_words_is_lt_five_percent, test_setup),
  };

  return cmocka_run_group_tests_name("StorageTest", tests, NULL, NULL);
}

int main() {
  return RunStorageTest();
}


