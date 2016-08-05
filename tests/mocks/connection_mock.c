#include <connection.h>

#include "cmocka_includes.h"

void connections_initialize(void) { }

connection* set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress) {
  check_expected(connectionHandle);
  return mock_ptr_type(connection *);
}

connection* set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress) {
  check_expected(connectionHandle);
  return mock_ptr_type(connection *);
}

ConnectionType unset_connection(uint16_t connectionHandle) {
  check_expected(connectionHandle);
  return mock_type(ConnectionType);
}

connection* find_active_connection_by_handle(uint16_t connectionHandle) {
  return mock_ptr_type(connection *);
}

bool central_connection_active(void) {
  return mock_type(bool);
}

bool peripheral_connections_active(void) {
  return mock_type(bool);
}

bool all_peripheral_connections_active(void) {
  return mock_type(bool);
}

char *get_connection_info(connection *conn, char *result) {
  return mock_type(char *);
}

void print_all_connections(char** parsedCommandArray) {
  check_expected_ptr(parsedCommandArray);
}

MessagePropagationType update_connection_info(uint16_t connectionHandle, uint8_t *dataPacket) {
  check_expected(connectionHandle);
  check_expected_ptr(dataPacket);
  return DoNotPropagate;
}

void share_connection_info(connection *targetConnection) { }