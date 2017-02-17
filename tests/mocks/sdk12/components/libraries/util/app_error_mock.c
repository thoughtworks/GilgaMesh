#include <app_error.h>
#include "cmocka_includes.h"

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {}

void app_error_handler_bare(ret_code_t error_code) {}

void app_error_save_and_stop(uint32_t id, uint32_t pc, uint32_t info) {}

static __INLINE void app_error_log(uint32_t id, uint32_t pc, uint32_t info) {}

static __INLINE void app_error_print(uint32_t id, uint32_t pc, uint32_t info) {}