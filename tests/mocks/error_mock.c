#include "error.h"
#include "cmocka_includes.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) { }

bool execute_succeeds(ret_code_t return_code) {
  return true;
}
