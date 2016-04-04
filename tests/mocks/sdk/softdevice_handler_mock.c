#include <softdevice_handler.h>

#include "cmocka_includes.h"

uint32_t softdevice_handler_init(nrf_clock_lfclksrc_t              clock_source,
                                 void *                            p_ble_evt_buffer,
                                 uint16_t                          ble_evt_buffer_size,
                                 softdevice_evt_schedule_func_t    evt_schedule_func)
{
   return mock_type(uint32_t);
}

uint32_t softdevice_handler_sd_disable(void) {
   return mock_type(uint32_t);
}

uint32_t softdevice_sys_evt_handler_set(sys_evt_handler_t sys_evt_handler) {
   return mock_type(uint32_t);
}

void intern_softdevice_events_execute(void) {

}


