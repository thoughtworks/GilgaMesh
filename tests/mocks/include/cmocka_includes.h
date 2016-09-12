#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <cmocka.h>

/* Family selection for family includes. */
//#if defined (NRF51)
//   #include <nrf51.h>
//   #include <nrf51_bitfields.h>
//   #include <nrf51_deprecated.h>
//#elif defined (NRF52)
//   #include <nrf52.h>
//   #include <nrf52_bitfields.h>
//   #include <nrf51_to_nrf52.h>
//   #include <nrf52_name_change.h>
//#else
//   #error "Device family must be defined. See nrf.h."
//#endif /* NRF51, NRF52 */

#include <compiler_abstraction.h>

// some color constants for debug output
#define COLOR_NRM "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GRN "\x1B[32m"
#define COLOR_YEL "\x1B[33m"
#define COLOR_BLU "\x1B[34m"
#define COLOR_MAG "\x1B[35m"
#define COLOR_CYN "\x1B[36m"
#define COLOR_WHT "\x1B[37m"
