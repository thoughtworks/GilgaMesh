#include <nrf_log.h>
#include "cmocka_includes.h"

void nrf_log_frontend_std_0(uint8_t severity, char const * const p_str) {}

void nrf_log_frontend_std_1(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0) {}

void nrf_log_frontend_std_2(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1) {}

void nrf_log_frontend_std_3(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2) {}

void nrf_log_frontend_std_4(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2,
                            uint32_t           val3) {}

void nrf_log_frontend_std_5(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2,
                            uint32_t           val3,
                            uint32_t           val4) {}

void nrf_log_frontend_std_6(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2,
                            uint32_t           val3,
                            uint32_t           val4,
                            uint32_t           val5) {}

void nrf_log_frontend_hexdump(uint8_t            severity,
                              char const * const p_str,
                              const void * const p_data,
                              uint16_t           length) {}

uint8_t nrf_log_getchar(void) {
   return mock_type(uint8_t);
}

