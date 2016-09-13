#pragma once

#include <stdbool.h>
#include <nrf_log.h>

#define MESH_LOG_DEBUG(...) if (!in_user_input_mode()) NRF_LOG_DEBUG( __VA_ARGS__);
#define MESH_LOG_INFO(...) if (!in_user_input_mode()) NRF_LOG_INFO( __VA_ARGS__);
#define MESH_LOG_WARNING(...) if (!in_user_input_mode()) NRF_LOG_WARNING( __VA_ARGS__);
#define MESH_LOG_ERROR(...) if (!in_user_input_mode()) NRF_LOG_ERROR( __VA_ARGS__);

#define MESH_LOG(...) if (!in_user_input_mode()) NRF_LOG_RAW_INFO( __VA_ARGS__); // log with no prefix

void set_user_input_mode(bool newValue);
bool in_user_input_mode();
