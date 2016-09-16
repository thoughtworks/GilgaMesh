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

static void BroadcastMessage_receive_propagates_to_all_connections() {
  assert_int_equal(PropagateToAll, receive_broadcast_message(0, &mockRequest));
}

static void BroadcastMessage_send_propagates_to_all_connections() {
  char* mockCommandArray[] = {"bro", "hey"};
  memcpy(mockRequest.message, mockCommandArray[1], strlen(mockCommandArray[1])+1);

  expect_value(send_to_all_connections, originHandle, BLE_CONN_HANDLE_INVALID);
  expect_memory(send_to_all_connections, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_all_connections, dataLength, sizeof(mockRequest));
  send_broadcast_message(mockCommandArray, 0);
}

static void BroadcastMessage_send_fails_if_message_is_too_long() {
  char* mockCommandArray[] = {"bro", "1234567890123"};

  // no expectations, message should not be sent
  send_broadcast_message(mockCommandArray, 0);
}

int RunBroadcastMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(BroadcastMessage_receive_propagates_to_all_connections, test_setup),
          cmocka_unit_test_setup(BroadcastMessage_send_propagates_to_all_connections, test_setup),
          cmocka_unit_test_setup(BroadcastMessage_send_fails_if_message_is_too_long, test_setup),
  };

  return cmocka_run_group_tests_name("BroadcastMessageTest", tests, NULL, NULL);
}

int main() {
  return RunBroadcastMessageTest();
}



