#include "message_types/handshake_message.h"

#include "cmocka_includes.h"
#include "version.h"

static BleMessageHandshakeReq mockRequest;
static connection mockConnection;
static uint32_t deviceId;

static int test_setup(void **state) {
  deviceId = get_raw_device_id();
  memset(&mockConnection, 0, sizeof(mockConnection));
  memset(&mockRequest, 0, sizeof(mockRequest));
  mockRequest.head.messageType = Handshake;
  mockRequest.majorVersion = APP_VERSION_MAIN;
  mockRequest.minorVersion = APP_VERSION_SUB;
  mockRequest.deviceId = deviceId;
  return 0;
}

static void test_send_handshake_message_sends_data_to_one_connection() {
  expect_value(send_to_single_connection, targetConnection, &mockConnection);
  expect_memory(send_to_single_connection, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_single_connection, dataLength, sizeof(mockRequest));
  send_handshake_message(&mockConnection);
}

static void test_receive_handshake_message_does_not_fail_when_connection_is_not_found() {
  will_return(find_active_connection_by_handle, NULL);

  assert_int_equal(DoNotPropagate, receive_handshake_message(0, &mockRequest));
}

static void test_receive_handshake_message_sets_data_for_connection() {
  will_return(find_active_connection_by_handle, &mockConnection);

  assert_int_equal(DoNotPropagate, receive_handshake_message(0, &mockRequest));
  assert_int_equal(APP_VERSION_MAIN, mockConnection.majorVersion);
  assert_int_equal(APP_VERSION_SUB, mockConnection.minorVersion);
  assert_int_equal(deviceId, mockConnection.deviceId);
}

int RunHandshakeMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(test_send_handshake_message_sends_data_to_one_connection, test_setup),
          cmocka_unit_test_setup(test_receive_handshake_message_does_not_fail_when_connection_is_not_found, test_setup),
          cmocka_unit_test_setup(test_receive_handshake_message_sets_data_for_connection, test_setup),
  };

  return cmocka_run_group_tests_name("HandshakeMessageTest", tests, NULL, NULL);
}

int main() {
  return RunHandshakeMessageTest();
}



