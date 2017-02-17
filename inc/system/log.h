#pragma once

#include <stdbool.h>
#include <nrf_log.h>

#define COLOR_CODE_DEFAULT "\x1B[0m"
#define COLOR_CODE_BLACK   "\x1B[1;30m"
#define COLOR_CODE_RED     "\x1B[1;31m"
#define COLOR_CODE_GREEN   "\x1B[1;32m"
#define COLOR_CODE_YELLOW  "\x1B[1;33m"
#define COLOR_CODE_BLUE    "\x1B[1;34m"
#define COLOR_CODE_MAGENTA "\x1B[1;35m"
#define COLOR_CODE_CYAN    "\x1B[1;36m"
#define COLOR_CODE_WHITE   "\x1B[1;37m"

#define MESH_LOG(...) if (!in_user_input_mode()) NRF_LOG_RAW_INFO( __VA_ARGS__); // log with no prefix
#define MESH_LOG_HEXDUMP(p_data, len) if (!in_user_input_mode()) NRF_LOG_RAW_HEXDUMP_INFO(p_data, len); // hexdup with no prefix

#define MESH_LOG_DEBUG(...) if (!in_user_input_mode()) NRF_LOG_DEBUG( __VA_ARGS__);
#define MESH_LOG_INFO(...) if (!in_user_input_mode()) NRF_LOG_INFO( __VA_ARGS__);
#define MESH_LOG_WARNING(...) if (!in_user_input_mode()) { NRF_LOG_WARNING( __VA_ARGS__ ); MESH_LOG(COLOR_CODE_DEFAULT); }
#define MESH_LOG_ERROR(...) if (!in_user_input_mode()) { NRF_LOG_ERROR( __VA_ARGS__ ); MESH_LOG(COLOR_CODE_DEFAULT); }

#define MESH_LOG_HEXDUMP_DEBUG(p_data, len) if (!in_user_input_mode()) NRF_LOG_HEXDUMP_DEBUG(p_data, len);
#define MESH_LOG_HEXDUMP_INFO(p_data, len) if (!in_user_input_mode()) NRF_LOG_HEXDUMP_INFO(p_data, len);
#define MESH_LOG_HEXDUMP_WARNING(p_data, len) if (!in_user_input_mode()) NRF_LOG_HEXDUMP_WARNING(p_data, len);
#define MESH_LOG_HEXDUMP_ERROR(p_data, len) if (!in_user_input_mode()) NRF_LOG_HEXDUMP_ERROR(p_data, len);

void set_user_input_mode(bool newValue);
bool in_user_input_mode();
