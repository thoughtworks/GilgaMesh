#pragma once

#if defined(TESTING) && !defined(DFU_TEST)
#include "dfu_mock.h"

#else

#include "nrf.h"

void enter_dfu();

#endif