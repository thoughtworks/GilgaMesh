
#include "cmocka_includes.h"
#include "connection.h"

static void CreateCentralConnection() {
//   uint16_t centralConId = 3;
//   ble_gap_addr_t centralAddress;
//   centralAddress.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
//   centralAddress.addr[0] = 0xca;
//   centralAddress.addr[1] = 0xfe;
//
//   connections_initialize();
//   set_central_connection(centralConId, centralAddress);
//   print_all_connections();
}

static void connectionTestTwo() {
   assert_false(false);
}

int RunConnectionTest(void) {
   const struct CMUnitTest tests[] = {
      cmocka_unit_test(CreateCentralConnection),
      cmocka_unit_test(connectionTestTwo),
   };

   //set test suite name here
   return cmocka_run_group_tests_name("ConnectionTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
   return RunConnectionTest();
}