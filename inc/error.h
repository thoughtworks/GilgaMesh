#pragma once

#include <nrf_soc.h>
#include <app_error.h>

#include <logger.h>

#define EC(ERROR_CODE)    app_error_handler((ERROR_CODE), __LINE__, (uint8_t*) __FILE__);

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
