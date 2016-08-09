#include "message_types/broadcast_message.h"

#include <ble_types.h>
#include "cmocka_includes.h"
#include "version.h"
#include "connection.h"

static BleMessageBroadcastReq mockRequest;

static int test_setup(void **state) {
  memset(&mockRequest, 0, sizeof(mockRequest));
  mockRequest.head.messageType = Broadcast;
  return 0;
}

static void test_receive_broadcast_message_propagates_to_all() {
  assert_int_equal(PropagateToAll, receive_broadcast_message(0, &mockRequest));
}

static void test_send_broadcast_message_sends_data_to_all_connections() {
  char* mockCommandArray[] = {"bro", "hey"};
  memcpy(mockRequest.message, mockCommandArray[1], strlen(mockCommandArray[1])+1);

  expect_value(send_to_all_connections, originHandle, BLE_CONN_HANDLE_INVALID);
  expect_memory(send_to_all_connections, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_all_connections, dataLength, sizeof(mockRequest));
  send_broadcast_message(mockCommandArray);
}

static void test_send_broadcast_message_fails_if_message_too_long() {
  char* mockCommandArray[] = {"bro", "1234567890123"};

  // no expectations, message should not be sent
  send_broadcast_message(mockCommandArray);
}

int RunBroadcastMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(test_receive_broadcast_message_propagates_to_all, test_setup),
          cmocka_unit_test_setup(test_send_broadcast_message_sends_data_to_all_connections, test_setup),
          cmocka_unit_test_setup(test_send_broadcast_message_fails_if_message_too_long, test_setup),
  };

  return cmocka_run_group_tests_name("BroadcastMessageTest", tests, NULL, NULL);
}

int main() {
  return RunBroadcastMessageTest();
}



