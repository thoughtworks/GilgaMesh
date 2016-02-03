#pragma once

#include <stdlib.h>
#include <malloc.h>

#include <nrf_soc.h>
#include <nrf_delay.h>
#include <app_error.h>
#include <softdevice_handler.h>

#include <mesh.h>
#include <logger.h>
#include <terminal.h>
#include <timer.h>

#define ATTR_TABLE_MAX_SIZE                 0x200
#define IS_SRVC_CHANGED_CHARACT_PRESENT     1
#define ATTR_MESH_CONNECTION_INTERVAL       MSEC_TO_UNITS(100, UNIT_1_25_MS)
#define ATTR_MESH_CONNECTION_TIMEOUT        MSEC_TO_UNITS(6000, UNIT_10_MS)

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
void sys_evt_dispatch(uint32_t sys_evt);
