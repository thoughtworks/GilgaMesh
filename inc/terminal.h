#pragma once

#include <simple_uart.h>
#include <stdbool.h>
#include "command.h"
#include "conversion.h"
#include "version.h"

#define TERMINAL_PROMPT "\r\n#> "
#define READ_BUFFER_SIZE 48
#define MAX_ARGUMENTS 8
#define TERMINAL_READ_TIMEOUT_US 10

void terminal_initialize(void);
void terminal_putstring(char* string);
uint8_t terminal_get(void);
void terminal_process_input(void);
void log_to_uart(char *logMessage, uint16_t messageLength);
