#include "cmocka_includes.h"
#include <fds.h>

bool fds_init_called = false;
bool fds_delete_called = false;
bool fds_write_called = false;
bool fds_open_called = false;
bool fds_close_called = false;

ret_code_t fds_register(fds_cb_t cb) {
  check_expected(cb);
  return FDS_SUCCESS;
}
ret_code_t fds_init(void) {
  fds_init_called = true;
  return FDS_SUCCESS;
}

ret_code_t fds_record_write(fds_record_desc_t* const p_desc, fds_record_t const* const p_record) {
  fds_write_called = true;
  return FDS_SUCCESS;
}

ret_code_t fds_record_delete(fds_record_desc_t * const p_desc) {
  fds_delete_called = true;
  return FDS_SUCCESS;
}

ret_code_t fds_record_find(uint16_t file_id, uint16_t record_key, fds_record_desc_t* const p_desc, fds_find_token_t* const p_token) {
  return mock_type(ret_code_t);
}

ret_code_t fds_record_open(fds_record_desc_t* const p_desc,fds_flash_record_t* const p_flash_record) {
  p_flash_record->p_data = "foo";
  fds_open_called = true;
  return mock_type(ret_code_t);
}

ret_code_t fds_record_close(fds_record_desc_t* const p_desc) {
  check_expected(p_desc);
  fds_close_called = true;
  return FDS_SUCCESS;
}


