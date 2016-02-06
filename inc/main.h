#pragma once

#include <stdlib.h>
#include <malloc.h>

#include <nrf_soc.h>
#include <app_error.h>
#include <softdevice_handler.h>

#include <connection.h>
#include <gap.h>
#include <logger.h>
#include <terminal.h>
#include <timer.h>

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
void sys_evt_dispatch(uint32_t sys_evt);
