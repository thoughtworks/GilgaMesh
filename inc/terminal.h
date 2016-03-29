#pragma once

#if defined(TESTING) && !defined(TERMINAL_TEST)
#include "terminal_mock.h"

#else

#include <simple_uart.h>
#include <boards.h>

void terminal_initialize(void);

#endif
