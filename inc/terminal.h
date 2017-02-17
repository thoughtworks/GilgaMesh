#pragma once

#include <simple_uart.h>
#include <stdbool.h>
#include "command.h"
#include "conversion.h"
#include "version.h"

#define TERMINAL_PROMPT "\r\n#> "
#define TERMINAL_BUFFER_SIZE 48
#define MAX_ARGUMENTS 8

bool terminal_initialize(void);
void terminal_putstring(char* string);
void terminal_process_input(void);
void terminal_execute_command(char* string);
