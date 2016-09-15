#include "app/custom_message_types/voting_heartbeat_message.h"

#include <ble_types.h>
#include "cmocka_includes.h"

static BleMessageVotingHeartbeatReq mockRequest;
static uint32_t deviceId;
static nfcStatus mockStatus = NFC_STATUS_WORKING;
static uint16_t mockVoteCount = 3;

static int test_setup(void **state) {
  deviceId = get_raw_device_id();
  memset(&mockRequest, 0, sizeof(mockRequest));
  mockRequest.head.messageType = 7;
  mockRequest.deviceId = deviceId;
  mockRequest.voteCount = mockVoteCount;
  mockRequest.nfcStatus = mockStatus;
  return 0;
}

static void VotingHeartbeat_send_message_sends_data_to_central_connection() {
  will_return(get_nfc_status, mockStatus);
  will_return(get_vote_count, mockVoteCount);

  expect_memory(send_to_central_connection, data, &mockRequest, sizeof(mockRequest));
  expect_value(send_to_central_connection, originHandle, BLE_CONN_HANDLE_INVALID);
  expect_value(send_to_central_connection, dataLength, sizeof(mockRequest));
  expect_any(get_vote_configuration, result);

  send_voting_heartbeat_message();
}

static void VotingHeartbeat_receive_message_propagates_to_central_connection() {
  assert_int_equal(PropagateToCentral, receive_voting_heartbeat_message(123, &mockRequest));
}

int RunVotingHeartbeatMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(VotingHeartbeat_send_message_sends_data_to_central_connection, test_setup),
          cmocka_unit_test_setup(VotingHeartbeat_receive_message_propagates_to_central_connection, test_setup),
  };

  return cmocka_run_group_tests_name("VotingHeartbeatMessageTest", tests, NULL, NULL);
}

int main() {
  return RunVotingHeartbeatMessageTest();
}

