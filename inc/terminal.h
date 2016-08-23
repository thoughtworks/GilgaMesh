#pragma once

#include <simple_uart.h>
#include <stdbool.h>
#include "command.h"
#include "conversion.h"
#include "version.h"

#define TERMINAL_PROMPT "\r\n#> "
#define COMMAND_MODE_ACK "#>\n"
#define READ_BUFFER_SIZE 128
#define MAX_ARGUMENTS 8
#define TERMINAL_READ_TIMEOUT_US 10

void terminal_initialize(void);
bool terminal_initialized();

void terminal_clear();

void terminal_put(const uint8_t character);
void terminal_putstring(char* string);

uint8_t terminal_get();
void terminal_get_with_timeout(uint8_t* character);

void terminal_process_input(void);

