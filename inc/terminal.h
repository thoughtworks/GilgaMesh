#pragma once

#include <simple_uart.h>
#include <boards.h>
#include <command.h>
#include <dfu.h>
#include <conversion.h>
#include <version.h>

#define TERMINAL_PROMPT "\r\n#> "
#define READ_BUFFER_SIZE 128
#define MAX_ARGUMENTS 3
#define TERMINAL_READ_TIMEOUT_US 10

void terminal_initialize(void);
bool terminal_initialized();
void terminal_process_input(void);

