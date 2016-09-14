#pragma once

#include <stdbool.h>
#include <nrf_log.h>

bool user_input_mode;
//#define MESH_LOG(...) (user_input_mode ? : NRF_LOG_INFO(__VA_ARGS__));
#define MESH_LOG(...)

void set_user_input_mode(bool newValue);
bool in_user_input_mode();
