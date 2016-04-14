#pragma once

#include <stdint.h>
#include <string.h>

#include <simple_uart.h>
#include <gatt.h>


#define COMMAND_SUCCESS 0


uint32_t command_execute(char** parsedCommandArray, uint8_t numberOfItems);
