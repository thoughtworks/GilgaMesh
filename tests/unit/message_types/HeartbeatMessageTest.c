#include "message_types/heartbeat_message.h"

#include <ble_types.h>
#include "cmocka_includes.h"
#include "version.h"
#include "connection.h"

static BleMessageHeartbeatReq mockRequest;

static int test_setup(void **state) {
  memset(&mockRequest, 0, sizeof(mockRequest));
  mockRequest.head.messageType = Heartbeat;
  mockRequest.majorVersion = APP_VERSION_MAIN;
  mockRequest.minorVersion = APP_VERSION_SUB;
  mockRequest.deviceId = deviceId;
  return 0;
}

static void test_send_heartbeat_message_sends_data_to_central_connection() {
  will_return(central_connection_active, false);

  expect_value(send_to_central_connection, originHandle, BLE_CONN_HANDLE_INVALID);
  expect_memory(send_to_central_connection, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_central_connection, dataLength, sizeof(mockRequest));
  send_heartbeat_message(NULL, 0);
}

static void test_send_heartbeat_message_includes_central_connection_info_if_present() {
  will_return(central_connection_active, true);

  uint32_t centralDeviceId = 12345;
  mockRequest.centralConnectionDeviceId = centralDeviceId;
  activeConnections = calloc(1, sizeof(connections));
  activeConnections->central.deviceId = centralDeviceId;

  expect_value(send_to_central_connection, originHandle, BLE_CONN_HANDLE_INVALID);
  expect_memory(send_to_central_connection, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_central_connection, dataLength, sizeof(mockRequest));
  send_heartbeat_message(NULL, 0);
}

static void test_send_heartbeat_message_does_not_include_central_connection_info_if_not_present() {
  will_return(central_connection_active, true);

  uint32_t centralDeviceId = 0;
  activeConnections = calloc(1, sizeof(connections));
  activeConnections->central.deviceId = centralDeviceId;

  expect_value(send_to_central_connection, originHandle, BLE_CONN_HANDLE_INVALID);
  expect_memory(send_to_central_connection, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_central_connection, dataLength, sizeof(mockRequest));
  send_heartbeat_message(NULL, 0);
}

static void test_receive_heartbeat_message_propagates_to_central_connection() {
  assert_int_equal(PropagateToCentral, receive_heartbeat_message(123, &mockRequest));
}

int RunHeartbeatMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(test_send_heartbeat_message_sends_data_to_central_connection, test_setup),
          cmocka_unit_test_setup(test_send_heartbeat_message_includes_central_connection_info_if_present, test_setup),
          cmocka_unit_test_setup(test_send_heartbeat_message_does_not_include_central_connection_info_if_not_present, test_setup),
          cmocka_unit_test_setup(test_receive_heartbeat_message_propagates_to_central_connection, test_setup),
  };

  return cmocka_run_group_tests_name("HeartbeatMessageTest", tests, NULL, NULL);
}

int main() {
  return RunHeartbeatMessageTest();
}

