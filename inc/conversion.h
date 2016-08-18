#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "message_types/message.h"

static const char HEX_DIGITS[16] = "0123456789ABCDEF";

uint8_t hex_digit_to_uint8(char digit);

