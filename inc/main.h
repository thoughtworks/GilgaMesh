#pragma once

#include <ble.h>

#define ATTR_TABLE_MAX_SIZE 0x200
#define IS_SRVC_CHANGED_CHARACT_PRESENT 1

void bleDispatchEventHandler(ble_evt_t * p_ble_evt);
void bleInit(void);
uint32_t initNodeID(void);
void initTimers(void);
static void timerEventDispatch(uint16_t passedTime, uint32_t appTimer);

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
void sys_evt_dispatch(uint32_t sys_evt);
