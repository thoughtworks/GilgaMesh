#pragma once

#include <ble.h>

#define ATTR_TABLE_MAX_SIZE 0x200
#define IS_SRVC_CHANGED_CHARACT_PRESENT 1
#define ATTR_TIMER_TICK_MS 200
#define ATTR_TIMER_COUNT 1
#define ATTR_TIMER_QUEUE_LENGTH 1
#define ATTR_TIMER_PRESCALER 0

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
void sys_evt_dispatch(uint32_t sys_evt);
