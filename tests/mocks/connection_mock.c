#include <connection.h>

#include "cmocka_includes.h"

void connections_initialize(void) { }

connection* set_central_connection(uint16_t connectionHandle) {
  check_expected(connectionHandle);
  return mock_ptr_type(connection *);
}

connection* set_peripheral_connection(uint16_t connectionHandle) {
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

bool is_connection_active(uint16_t connectionHandle) {
  return mock_type(bool);
}

uint32_t get_central_connection_device_id(void) {
  return mock_type(uint32_t);
}

void print_all_connections(char** parsedCommandArray) {
  check_expected_ptr(parsedCommandArray);
}
