#include <ble_l2cap.h>
#include "cmocka_includes.h"

uint32_t sd_ble_l2cap_cid_register(uint16_t cid) { return mock_type(uint32_t); }

uint32_t sd_ble_l2cap_cid_unregister(uint16_t cid) { return mock_type(uint32_t); }

uint32_t sd_ble_l2cap_tx(uint16_t conn_handle, ble_l2cap_header_t const *p_header, uint8_t const *p_data) { return mock_type(uint32_t); }