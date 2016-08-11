#pragma once
#include <nrf_drv_twi.h>

uint32_t sys_i2c_init(nrf_drv_twi_t const *        p_instance,
                      nrf_drv_twi_config_t const * p_config,
                      nrf_drv_twi_evt_handler_t    event_handler,
                      void *                       p_context);

void sys_i2c_uninit(nrf_drv_twi_t const * p_instance);

uint32_t sys_i2c_tx(nrf_drv_twi_t const * p_instance,
                          uint8_t               address,
                          uint8_t const *       p_data,
                          uint8_t               length,
                          bool                  no_stop);

uint32_t sys_i2c_rx(nrf_drv_twi_t const * p_instance,
                          uint8_t               address,
                          uint8_t *             p_data,
                          uint8_t               length);

uint32_t sys_i2c_xfer(nrf_drv_twi_t           const * p_instance,
                            nrf_drv_twi_xfer_desc_t const * p_xfer_desc,
                            uint32_t                        flags);

uint32_t sys_i2c_data_count(nrf_drv_twi_t const * const p_instance);



