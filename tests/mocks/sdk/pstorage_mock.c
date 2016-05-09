#include <pstorage.h>

#include "cmocka_includes.h"


uint32_t pstorage_init(void) {

}

uint32_t pstorage_register(pstorage_module_param_t * p_module_param, pstorage_handle_t *p_block_id) {
  check_expected(p_module_param);
}

uint32_t pstorage_store(pstorage_handle_t * p_dest, uint8_t *p_src, pstorage_size_t size, pstorage_size_t offset) {
  check_expected_ptr(p_src);
  check_expected(size);
}

uint32_t pstorage_load(uint8_t * p_dest, pstorage_handle_t *p_src, pstorage_size_t size, pstorage_size_t offset) {
  check_expected(size);
}

uint32_t pstorage_clear(pstorage_handle_t * p_base_id, pstorage_size_t size) {
  check_expected(size);
}
