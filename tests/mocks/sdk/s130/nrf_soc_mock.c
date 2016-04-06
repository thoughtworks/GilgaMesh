#include <nrf_soc.h>
#include "cmocka_includes.h"

uint32_t sd_mutex_new(nrf_mutex_t * p_mutex) { return mock_type(uint32_t); } 


uint32_t sd_mutex_acquire(nrf_mutex_t * p_mutex) { return mock_type(uint32_t); } 


uint32_t sd_mutex_release(nrf_mutex_t * p_mutex) { return mock_type(uint32_t); } 


uint32_t sd_nvic_EnableIRQ(IRQn_Type IRQn) { return mock_type(uint32_t); } 


uint32_t sd_nvic_DisableIRQ(IRQn_Type IRQn) { return mock_type(uint32_t); } 


uint32_t sd_nvic_GetPendingIRQ(IRQn_Type IRQn, uint32_t * p_pending_irq) { return mock_type(uint32_t); } 


uint32_t sd_nvic_SetPendingIRQ(IRQn_Type IRQn) { return mock_type(uint32_t); } 


uint32_t sd_nvic_ClearPendingIRQ(IRQn_Type IRQn) { return mock_type(uint32_t); } 


uint32_t sd_nvic_SetPriority(IRQn_Type IRQn, nrf_app_irq_priority_t priority) { return mock_type(uint32_t); } 


uint32_t sd_nvic_GetPriority(IRQn_Type IRQn, nrf_app_irq_priority_t * p_priority) { return mock_type(uint32_t); } 


uint32_t sd_nvic_SystemReset(void) { return mock_type(uint32_t); } 


uint32_t sd_nvic_critical_region_enter(uint8_t * p_is_nested_critical_region) { return mock_type(uint32_t); } 


uint32_t sd_nvic_critical_region_exit(uint8_t is_nested_critical_region) { return mock_type(uint32_t); } 


uint32_t sd_rand_application_pool_capacity_get(uint8_t * p_pool_capacity) { return mock_type(uint32_t); } 


uint32_t sd_rand_application_bytes_available_get(uint8_t * p_bytes_available) { return mock_type(uint32_t); } 


uint32_t sd_rand_application_vector_get(uint8_t * p_buff, uint8_t length) { return mock_type(uint32_t); } 


uint32_t sd_power_reset_reason_get(uint32_t * p_reset_reason) { return mock_type(uint32_t); } 


uint32_t sd_power_reset_reason_clr(uint32_t reset_reason_clr_msk) { return mock_type(uint32_t); } 


uint32_t sd_power_mode_set(nrf_power_mode_t power_mode) { return mock_type(uint32_t); } 


uint32_t sd_power_system_off(void) { return mock_type(uint32_t); } 


uint32_t sd_power_pof_enable(uint8_t pof_enable) { return mock_type(uint32_t); } 


uint32_t sd_power_pof_threshold_set(nrf_power_failure_threshold_t threshold) { return mock_type(uint32_t); } 


uint32_t sd_power_ramon_set(uint32_t ramon) { return mock_type(uint32_t); } 


uint32_t sd_power_ramon_clr(uint32_t ramon) { return mock_type(uint32_t); } 


uint32_t sd_power_ramon_get(uint32_t * p_ramon) { return mock_type(uint32_t); } 


uint32_t sd_power_gpregret_set(uint32_t gpregret_msk) { return mock_type(uint32_t); } 


uint32_t sd_power_gpregret_clr(uint32_t gpregret_msk) { return mock_type(uint32_t); } 


uint32_t sd_power_gpregret_get(uint32_t *p_gpregret) { return mock_type(uint32_t); } 


uint32_t sd_power_dcdc_mode_set(nrf_power_dcdc_mode_t dcdc_mode) { return mock_type(uint32_t); } 


uint32_t sd_clock_hfclk_request(void) { return mock_type(uint32_t); } 


uint32_t sd_clock_hfclk_release(void) { return mock_type(uint32_t); } 


uint32_t sd_clock_hfclk_is_running(uint32_t * p_is_running) { return mock_type(uint32_t); } 


uint32_t sd_app_evt_wait(void) { return mock_type(uint32_t); } 


uint32_t sd_ppi_channel_enable_get(uint32_t * p_channel_enable) { return mock_type(uint32_t); } 


uint32_t sd_ppi_channel_enable_set(uint32_t channel_enable_set_msk) { return mock_type(uint32_t); } 


uint32_t sd_ppi_channel_enable_clr(uint32_t channel_enable_clr_msk) { return mock_type(uint32_t); } 


uint32_t sd_ppi_channel_assign(uint8_t channel_num, const volatile void * evt_endpoint, const volatile void * task_endpoint) { return mock_type(uint32_t); } 


uint32_t sd_ppi_group_task_enable(uint8_t group_num) { return mock_type(uint32_t); } 


uint32_t sd_ppi_group_task_disable(uint8_t group_num) { return mock_type(uint32_t); } 


uint32_t sd_ppi_group_assign(uint8_t group_num, uint32_t channel_msk) { return mock_type(uint32_t); } 


uint32_t sd_ppi_group_get(uint8_t group_num, uint32_t * p_channel_msk) { return mock_type(uint32_t); } 


uint32_t sd_radio_notification_cfg_set(nrf_radio_notification_type_t type, nrf_radio_notification_distance_t distance) { return mock_type(uint32_t); } 


uint32_t sd_ecb_block_encrypt(nrf_ecb_hal_data_t * p_ecb_data) { return mock_type(uint32_t); } 


uint32_t sd_evt_get(uint32_t * p_evt_id) { return mock_type(uint32_t); } 


uint32_t sd_temp_get(int32_t * p_temp) { return mock_type(uint32_t); } 


uint32_t sd_flash_write(uint32_t * const p_dst, uint32_t const * const p_src, uint32_t size) { return mock_type(uint32_t); } 


uint32_t sd_flash_page_erase(uint32_t page_number) { return mock_type(uint32_t); } 


uint32_t sd_flash_protect(uint32_t protenset0, uint32_t protenset1) { return mock_type(uint32_t); } 


uint32_t sd_radio_session_open(nrf_radio_signal_callback_t p_radio_signal_callback) { return mock_type(uint32_t); } 


uint32_t sd_radio_session_close(void) { return mock_type(uint32_t); } 


uint32_t sd_radio_request(nrf_radio_request_t * p_request ) { return mock_type(uint32_t); } 

