#include <nrf_sdm.h>
#include "cmocka_includes.h"

uint32_t sd_softdevice_enable(nrf_clock_lf_cfg_t const * p_clock_lf_cfg, nrf_fault_handler_t fault_handler) { return mock_type(uint32_t); }

uint32_t sd_softdevice_disable(void) { return mock_type(uint32_t); }

uint32_t sd_softdevice_is_enabled(uint8_t * p_softdevice_enabled) { return mock_type(uint32_t); }

uint32_t sd_softdevice_vector_table_base_set(uint32_t address) { return mock_type(uint32_t); }
