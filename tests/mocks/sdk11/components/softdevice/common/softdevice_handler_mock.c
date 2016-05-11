#include <softdevice_handler.h>
#include "cmocka_includes.h"

bool softdevice_handler_isEnabled(void) {
   return mock_type(bool);
}

uint32_t softdevice_handler_init(nrf_clock_lf_cfg_t *              p_clock_lf_cfg,
                                 void *                            p_ble_evt_buffer,
                                 uint16_t                          ble_evt_buffer_size,
                                 softdevice_evt_schedule_func_t    evt_schedule_func)
{
   return mock_type(uint32_t);
}

uint32_t softdevice_handler_sd_disable(void){
   return mock_type(bool);
}

uint32_t softdevice_sys_evt_handler_set(sys_evt_handler_t sys_evt_handler){
   return mock_type(bool);
}

uint32_t softdevice_enable_get_default_config(uint8_t central_links_count,
                                              uint8_t periph_links_count,
                                              ble_enable_params_t * p_ble_enable_params)
{
   return mock_type(bool);
}

uint32_t softdevice_enable(ble_enable_params_t * p_ble_enable_params) {
   return mock_type(bool);
}
