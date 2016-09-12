#pragma once

#include <nrf_soc.h>
#include <system/error.h>

#include "logger.h"

#define EC(ERROR_CODE)    sys_error_handler((ERROR_CODE), __LINE__, (uint8_t*) __FILE__);

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info);

//Functions for resolving error codes
const char* getNrfErrorString(uint32_t nrfErrorCode);
const char* getHciErrorString(uint8_t hciErrorCode);
const char* getBleEventNameString(uint16_t bleEventId);
const char* getGattStatusErrorString(uint16_t gattStatusCode);
