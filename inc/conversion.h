#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "message.h"

#define INT_DIGITS 19/* enough for 64 bit integer */

static const char HEX_DIGITS[16] = "0123456789ABCDEF";

char* int_to_string(uint16_t i);
