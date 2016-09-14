#pragma once

#include <nrf_soc.h>
#include <system/error.h>


#define EC(ERROR_CODE)    app_error_handler((ERROR_CODE), __LINE__, (uint8_t*) __FILE__);

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info);

const char* getBleEventNameString(uint16_t bleEventId);

