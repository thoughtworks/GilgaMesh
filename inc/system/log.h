#pragma once

#include <stdbool.h>
#include <nrf_log.h>

static bool user_input_mode;
#define MESH_LOG_DEBUG(...) if (!user_input_mode) NRF_LOG_DEBUG( __VA_ARGS__);
#define MESH_LOG_INFO(...) if (!user_input_mode) NRF_LOG_INFO( __VA_ARGS__);
#define MESH_LOG_WARNING(...) if (!user_input_mode) NRF_LOG_WARNING( __VA_ARGS__);
#define MESH_LOG_ERROR(...) if (!user_input_mode) NRF_LOG_ERROR( __VA_ARGS__);

#define MESH_LOG(...) if (!user_input_mode) NRF_LOG_RAW_INFO( __VA_ARGS__); // log with no prefix

void set_user_input_mode(bool newValue);
bool in_user_input_mode();
