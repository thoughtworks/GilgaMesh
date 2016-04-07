#pragma once

#include <simple_uart.h>
#include <boards.h>
#include <dfu.h>
#include <conversion.h>
#include <version.h>

void terminal_initialize(void);
void terminal_process_input(void);

