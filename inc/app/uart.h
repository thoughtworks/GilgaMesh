#pragma once

#include <simple_uart.h>
#include <stdbool.h>
#include "command.h"
#include "conversion.h"
#include "version.h"

#define UART_BUFFER_SIZE 48

void uart_initialize(void);
void log_to_uart(char *logMessage, uint16_t messageLength);
