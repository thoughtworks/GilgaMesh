#include "message_types/heartbeat_message.h"

#include <ble_types.h>
#include "cmocka_includes.h"
#include "version.h"
#include "connection.h"

static BleMessageHeartbeatReq mockRequest;
static uint32_t deviceId;
static uint32_t centralDeviceId = 6789;

static int test_setup(void **state) {
  deviceId = get_raw_device_id();
  memset(&mockRequest, 0, sizeof(mockRequest));
  mockRequest.head.messageType = Heartbeat;
  mockRequest.majorVersion = APP_VERSION_MAIN;
  mockRequest.minorVersion = APP_VERSION_SUB;
  mockRequest.deviceId = deviceId;
  mockRequest.centralConnectionDeviceId = centralDeviceId;
  return 0;
}

static void HeartbeatMessage_send_propagates_to_central_connection() {
  will_return(get_central_connection_device_id, centralDeviceId);

  expect_memory(send_to_central_connection, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_central_connection, originHandle, BLE_CONN_HANDLE_INVALID);
  expect_value(send_to_central_connection, dataLength, sizeof(mockRequest));
  send_heartbeat_message(NULL, 0);
}

static void HeartbeatMessage_receive_propagates_to_central_connection() {
  assert_int_equal(PropagateToCentral, receive_heartbeat_message(123, &mockRequest));
}

int RunHeartbeatMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(HeartbeatMessage_send_propagates_to_central_connection, test_setup),
          cmocka_unit_test_setup(HeartbeatMessage_receive_propagates_to_central_connection, test_setup),
  };

  return cmocka_run_group_tests_name("HeartbeatMessageTest", tests, NULL, NULL);
}

int main() {
  return RunHeartbeatMessageTest();
}

