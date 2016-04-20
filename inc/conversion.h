#pragma once

#include <stdint.h>
#include <string.h>
#include <message.h>

#define INT_DIGITS 19/* enough for 64 bit integer */

char* int_to_string(uint16_t i);
void set_node_name_from_device_id(uint32_t deviceId, char *nodeName);
