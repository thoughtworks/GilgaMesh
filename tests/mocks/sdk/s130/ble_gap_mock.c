#include <ble_gap.h>
#include <ble.h>
#include <nrf_soc.h>

#include "cmocka_includes.h"

void tx_buf_alloc(uint8_t * * p_data, uint32_t * p_len)
{

}

uint32_t uuid_encode_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}


uint32_t sd_ble_uuid_encode(ble_uuid_t const * const p_uuid,
                            uint8_t * const          p_uuid_le_len,
                            uint8_t * const          p_uuid_le)
{
   return mock_type(uint32_t);
}

uint32_t tx_buffer_count_get_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_tx_buffer_count_get(uint8_t * p_count)
{
   return mock_type(uint32_t);
}

uint32_t uuid_vs_add_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_uuid_vs_add(ble_uuid128_t const * const p_vs_uuid, uint8_t * const p_uuid_type)
{
   return mock_type(uint32_t);
}

uint32_t uuid_decode_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_uuid_decode(uint8_t               uuid_le_len,
                            uint8_t const * const p_uuid_le,
                            ble_uuid_t * const    p_uuid)
{
   return mock_type(uint32_t);
}

uint32_t version_get_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_version_get(ble_version_t * p_version)
{
   return mock_type(uint32_t);
}

uint32_t opt_get_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_opt_get(uint32_t opt_id, ble_opt_t *p_opt)
{
   return mock_type(uint32_t);
}

uint32_t opt_set_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_opt_set(uint32_t opt_id, ble_opt_t const *p_opt)
{
   return mock_type(uint32_t);
}

uint32_t enable_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_enable(ble_enable_params_t * p_params)
{
   return mock_type(uint32_t);
}

uint32_t user_mem_reply_rsp_dec(const uint8_t * p_buffer, uint16_t length)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_user_mem_reply(uint16_t conn_handle, ble_user_mem_block_t const *p_block)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_gap_adv_data_set(uint8_t const * const p_data,
                                 uint8_t               dlen,
                                 uint8_t const * const p_sr_data,
                                 uint8_t               srdlen)
{
   return mock_type(uint32_t);
}

uint32_t sd_ble_gap_adv_start(ble_gap_adv_params_t const * const p_adv_params)
{
   return mock_type(uint32_t);
}

uint32_t sd_power_dcdc_mode_set(nrf_power_dcdc_mode_t dcdc_mode) { return mock_type(uint32_t); }
uint32_t sd_power_mode_set(nrf_power_mode_t power_mode) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_device_name_set(ble_gap_conn_sec_mode_t const *p_write_perm, uint8_t const *p_dev_name, uint16_t len ) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_appearance_set(uint16_t appearance) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_ppcp_set(ble_gap_conn_params_t const *p_conn_params) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_connect(ble_gap_addr_t const *p_peer_addr, ble_gap_scan_params_t const *p_scan_params, ble_gap_conn_params_t const *p_conn_params) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_scan_stop(void) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_scan_start(ble_gap_scan_params_t const *p_scan_params) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_adv_stop(void) { return mock_type(uint32_t); }
uint32_t sd_ble_gap_disconnect(uint16_t conn_handle, uint8_t hci_status_code) { return mock_type(uint32_t); }
