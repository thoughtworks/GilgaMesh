#include <nrf_soc.h>
#include "cmocka_includes.h"

uint32_t sd_app_evt_wait(void) { return mock_type(uint32_t); }

uint32_t sd_ppi_channel_enable_get(uint32_t * p_channel_enable) { return mock_type(uint32_t); }

uint32_t sd_ppi_channel_enable_set(uint32_t channel_enable_set_msk) { return mock_type(uint32_t); }

uint32_t sd_ppi_channel_enable_clr(uint32_t channel_enable_clr_msk) { return mock_type(uint32_t); }

uint32_t sd_ppi_channel_assign(uint8_t channel_num, const volatile void * evt_endpoint, const volatile void * task_endpoint) { return mock_type(uint32_t); }

uint32_t sd_ppi_group_task_enable(uint8_t group_num) { return mock_type(uint32_t); }

uint32_t sd_ppi_group_task_disable(uint8_t group_num) { return mock_type(uint32_t); }

uint32_t sd_ppi_group_assign(uint8_t group_num, uint32_t channel_msk) { return mock_type(uint32_t); }

uint32_t sd_ppi_group_get(uint8_t group_num, uint32_t * p_channel_msk) { return mock_type(uint32_t); }

uint32_t sd_radio_notification_cfg_set(uint8_t type, uint8_t distance) { return mock_type(uint32_t); }

uint32_t sd_ecb_block_encrypt(nrf_ecb_hal_data_t * p_ecb_data) { return mock_type(uint32_t); }

uint32_t sd_ecb_blocks_encrypt(uint8_t block_count, nrf_ecb_hal_data_block_t * p_data_blocks) { return mock_type(uint32_t); }

uint32_t sd_evt_get(uint32_t * p_evt_id) { return mock_type(uint32_t); }

uint32_t sd_temp_get(int32_t * p_temp) { return mock_type(uint32_t); }

uint32_t sd_flash_write(uint32_t * const p_dst, uint32_t const * const p_src, uint32_t size) { return mock_type(uint32_t); }

uint32_t sd_flash_page_erase(uint32_t page_number) { return mock_type(uint32_t); }

uint32_t sd_flash_protect(uint32_t block_cfg0, uint32_t block_cfg1, uint32_t block_cfg2, uint32_t block_cfg3) { return mock_type(uint32_t); }

uint32_t sd_radio_session_open(nrf_radio_signal_callback_t p_radio_signal_callback) { return mock_type(uint32_t); }

uint32_t sd_radio_session_close(void) { return mock_type(uint32_t); }

uint32_t sd_radio_request(nrf_radio_request_t * p_request ) { return mock_type(uint32_t); }
