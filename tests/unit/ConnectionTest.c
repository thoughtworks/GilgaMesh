#include "cmocka_includes.h"
#include "connection.h"

static void Connection_central_connection_active_returns_true_when_parent_exists() {
  will_return(ble_conn_state_n_peripherals, 1);
  assert_true(central_connection_active());
}

static void Connection_central_connection_active_returns_false_when_parent_does_not_exist() {
  will_return(ble_conn_state_n_peripherals, 0);
  assert_false(central_connection_active());
}

static void Connection_all_peripheral_connections_active_returns_true_when_conns_maxed_out() {
  will_return(ble_conn_state_n_centrals, MAX_PERIPHERAL_CONNECTIONS);
  assert_true(all_peripheral_connections_active());
}

static void Connection_all_peripheral_connections_active_returns_false_when_conns_not_maxed_out() {
  will_return(ble_conn_state_n_centrals, MAX_PERIPHERAL_CONNECTIONS-1);
  assert_false(all_peripheral_connections_active());
}

static void Connection_set_connection_sets_data_in_active_connections() {
  uint16_t connectionHandle = 1234;
  ConnectionType connectionType = CENTRAL;

  connections_initialize();
  connection* result = set_connection(connectionHandle, connectionType);
  assert_int_equal(result->type, connectionType);
  assert_int_equal(result->handle, connectionHandle);
  assert_true(result->active);
}

int RunConnectionTest(void) {
   const struct CMUnitTest tests[] = {
      cmocka_unit_test(Connection_central_connection_active_returns_true_when_parent_exists),
      cmocka_unit_test(Connection_central_connection_active_returns_false_when_parent_does_not_exist),
      cmocka_unit_test(Connection_all_peripheral_connections_active_returns_true_when_conns_maxed_out),
      cmocka_unit_test(Connection_all_peripheral_connections_active_returns_false_when_conns_not_maxed_out),
      cmocka_unit_test(Connection_set_connection_sets_data_in_active_connections),
   };

   //set test suite name here
   return cmocka_run_group_tests_name("ConnectionTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
   return RunConnectionTest();
}