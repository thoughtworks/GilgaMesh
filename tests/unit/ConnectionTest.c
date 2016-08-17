
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

int RunConnectionTest(void) {
   const struct CMUnitTest tests[] = {
      cmocka_unit_test(Connection_central_connection_active_returns_true_when_parent_exists),
      cmocka_unit_test(Connection_central_connection_active_returns_false_when_parent_does_not_exist),
      cmocka_unit_test(Connection_all_peripheral_connections_active_returns_true_when_conns_maxed_out),
      cmocka_unit_test(Connection_all_peripheral_connections_active_returns_false_when_conns_not_maxed_out),
   };

   //set test suite name here
   return cmocka_run_group_tests_name("ConnectionTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
   return RunConnectionTest();
}