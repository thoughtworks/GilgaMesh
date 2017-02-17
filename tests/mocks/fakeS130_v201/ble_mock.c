#include <ble.h>
#include "cmocka_includes.h"

uint32_t sd_ble_enable(ble_enable_params_t * p_ble_enable_params, uint32_t * p_app_ram_base) { return mock_type(uint32_t); }

uint32_t sd_ble_evt_get(uint8_t *p_dest, uint16_t *p_len) { return mock_type(uint32_t); }

uint32_t sd_ble_tx_packet_count_get(uint16_t conn_handle, uint8_t *p_count) { return mock_type(uint32_t); }

uint32_t sd_ble_uuid_vs_add(ble_uuid128_t const *p_vs_uuid, uint8_t *p_uuid_type) { return mock_type(uint32_t); }

uint32_t sd_ble_uuid_decode(uint8_t uuid_le_len, uint8_t const *p_uuid_le, ble_uuid_t *p_uuid) { return mock_type(uint32_t); }

uint32_t sd_ble_uuid_encode(ble_uuid_t const *p_uuid, uint8_t *p_uuid_le_len, uint8_t *p_uuid_le) { return mock_type(uint32_t); }

uint32_t sd_ble_version_get(ble_version_t *p_version) { return mock_type(uint32_t); }

uint32_t sd_ble_user_mem_reply(uint16_t conn_handle, ble_user_mem_block_t const *p_block) { return mock_type(uint32_t); }

uint32_t sd_ble_opt_set(uint32_t opt_id, ble_opt_t const *p_opt) { return mock_type(uint32_t); }

uint32_t sd_ble_opt_get(uint32_t opt_id, ble_opt_t *p_opt) { return mock_type(uint32_t); }
