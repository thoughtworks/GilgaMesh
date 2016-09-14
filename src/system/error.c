#include <system/error.h>
#include <app_error.h>

void sys_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    app_error_handler(error_code, line_num, p_file_name);
}
