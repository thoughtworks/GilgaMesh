#pragma once

#include <nrf_soc.h>
#include <app_error.h>

#include <logger.h>

#define EC(ERROR_CODE)    APP_ERROR_CHECK(ERROR_CODE);

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    // TODO: print debug out if we end up here
}
