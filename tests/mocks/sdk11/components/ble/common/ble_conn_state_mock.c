#include <ble_conn_state.h>
#include "cmocka_includes.h"

uint32_t ble_conn_state_n_centrals(void) {
  return mock_type(uint32_t);
}

uint32_t ble_conn_state_n_peripherals(void) {
  return mock_type(uint32_t);
}

uint32_t ble_conn_state_n_connections(void) {
  return mock_type(uint32_t);
}

sdk_mapped_flags_key_list_t ble_conn_state_conn_handles(void) {
  sdk_mapped_flags_key_list_t result;
  result.len = mock_type(uint32_t);
  result.flag_keys[0] = mock_type(uint16_t);
  return result;
}

uint8_t ble_conn_state_role(uint16_t conn_handle) {
  return mock_type(uint8_t);
}

ble_conn_state_status_t ble_conn_state_status(uint16_t conn_handle) {
  return mock_type(ble_conn_state_status_t);
}