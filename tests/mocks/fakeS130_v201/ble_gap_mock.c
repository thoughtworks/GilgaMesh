#include <ble_gap.h>
#include "cmocka_includes.h"

uint32_t sd_ble_gap_address_set(uint8_t addr_cycle_mode, ble_gap_addr_t const *p_addr) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_address_get(ble_gap_addr_t *p_addr) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_adv_data_set(uint8_t const *p_data, uint8_t dlen, uint8_t const *p_sr_data, uint8_t srdlen) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_adv_start(ble_gap_adv_params_t const *p_adv_params) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_adv_stop(void) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_conn_param_update(uint16_t conn_handle, ble_gap_conn_params_t const *p_conn_params) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_disconnect(uint16_t conn_handle, uint8_t hci_status_code) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_tx_power_set(int8_t tx_power) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_appearance_set(uint16_t appearance) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_appearance_get(uint16_t *p_appearance) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_ppcp_set(ble_gap_conn_params_t const *p_conn_params) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_ppcp_get(ble_gap_conn_params_t *p_conn_params) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_device_name_set(ble_gap_conn_sec_mode_t const *p_write_perm, uint8_t const *p_dev_name, uint16_t len) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_device_name_get(uint8_t *p_dev_name, uint16_t *p_len) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_authenticate(uint16_t conn_handle, ble_gap_sec_params_t const *p_sec_params) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_sec_params_reply(uint16_t conn_handle, uint8_t sec_status, ble_gap_sec_params_t const *p_sec_params, ble_gap_sec_keyset_t const *p_sec_keyset) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_auth_key_reply(uint16_t conn_handle, uint8_t key_type, uint8_t const *p_key) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_lesc_dhkey_reply(uint16_t conn_handle, ble_gap_lesc_dhkey_t const *p_dhkey) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_keypress_notify(uint16_t conn_handle, uint8_t kp_not) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_lesc_oob_data_get(uint16_t conn_handle, ble_gap_lesc_p256_pk_t const *p_pk_own, ble_gap_lesc_oob_data_t *p_oobd_own) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_lesc_oob_data_set(uint16_t conn_handle, ble_gap_lesc_oob_data_t const *p_oobd_own, ble_gap_lesc_oob_data_t const *p_oobd_peer) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_encrypt(uint16_t conn_handle, ble_gap_master_id_t const *p_master_id, ble_gap_enc_info_t const *p_enc_info) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_sec_info_reply(uint16_t conn_handle, ble_gap_enc_info_t const *p_enc_info, ble_gap_irk_t const *p_id_info, ble_gap_sign_info_t const *p_sign_info) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_conn_sec_get(uint16_t conn_handle, ble_gap_conn_sec_t *p_conn_sec) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_rssi_start(uint16_t conn_handle, uint8_t threshold_dbm, uint8_t skip_count) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_rssi_stop(uint16_t conn_handle) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_rssi_get(uint16_t conn_handle, int8_t *p_rssi) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_scan_start(ble_gap_scan_params_t const *p_scan_params) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_scan_stop(void) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_connect(ble_gap_addr_t const *p_peer_addr, ble_gap_scan_params_t const *p_scan_params, ble_gap_conn_params_t const *p_conn_params) { return mock_type(uint32_t); }

uint32_t sd_ble_gap_connect_cancel(void) { return mock_type(uint32_t); }

