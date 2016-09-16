#include "app/custom_message_types/vote_ack_message.h"

#include "cmocka_includes.h"
#include <ble_types.h>

extern bool did_broadcast_next_vote;
static BleMessageVoteAckReq mockRequest;
static BleMessageType mockMessageType = 123;
static uint32_t deviceId;
static uint32_t otherDeviceId = 0x3456;
userVote mockVote;

static int test_setup(void **state) {
  did_broadcast_next_vote = false;
  deviceId = get_raw_device_id();
  memset(&mockRequest, 0, sizeof(mockRequest));
  mockRequest.messageType = mockMessageType;

  return 0;
}

static void VoteAckMessage_initialize_registers_message_and_adds_terminal_command() {
  will_return(register_message_type, mockMessageType);

  expect_string(mesh_add_terminal_command, commandName, "vack");
  vote_ack_message_initialize();
}

static void VoteAckMessage_receive_propagates_to_all_when_current_node_is_not_target() {
  mockRequest.deviceId = otherDeviceId;

  assert_int_equal(PropagateToAll, receive_vote_acknowledgement(0, &mockRequest));
}

static void VoteAckMessage_receive_does_not_propagate_when_current_node_is_target() {
  mockRequest.deviceId = deviceId;
  will_return(get_data_from_storage, NULL);

  assert_int_equal(DoNotPropagate, receive_vote_acknowledgement(0, &mockRequest));
}

static void VoteAckMessage_receive_deletes_correct_vote_from_storage_when_timestamp_matches() {
  mockRequest.deviceId = deviceId;
  will_return(get_data_from_storage, &mockVote);
  will_return(rtc_is_equal_timestamp, true);

  expect_value(delete_data_from_storage, fileId, VOTES_STORAGE_FILE_ID);
  expect_value(delete_data_from_storage, recordKey, mockVote.voterId);
  receive_vote_acknowledgement(0, &mockRequest);
}

static void VoteAckMessage_receive_does_not_delete_vote_from_storage_when_timestamps_do_not_match() {
  mockRequest.deviceId = deviceId;
  will_return(get_data_from_storage, &mockVote);
  will_return(rtc_is_equal_timestamp, false);

  receive_vote_acknowledgement(0, &mockRequest);
}

static void VoteAckMessage_receive_broadcasts_next_vote_when_current_node_is_target() {
  mockRequest.deviceId = deviceId;
  will_return(get_data_from_storage, NULL);

  receive_vote_acknowledgement(0, &mockRequest);
  assert_true(did_broadcast_next_vote);
}

static void VoteAckMessage_broadcast_sends_to_all_connections() {
  char* voterIdStr = "1";
  mockRequest.voteAck.voterId = 1;

  char* nodeIdStr = "1234";
  mockRequest.deviceId = 1234;

  char* timestampStr = "anytimestamp";

  expect_string(rtc_timestring_to_timestamp, string, timestampStr);
  expect_any(rtc_timestring_to_timestamp, stamp);

  expect_memory(send_to_all_connections, data, &mockRequest, sizeof(mockRequest));
  expect_any(send_to_all_connections, dataLength);
  expect_value(send_to_all_connections, originHandle, BLE_CONN_HANDLE_INVALID);

  broadcast_vote_acknowledgement(nodeIdStr, voterIdStr, timestampStr);
}

int RunVoteAckMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(VoteAckMessage_initialize_registers_message_and_adds_terminal_command, test_setup),
          cmocka_unit_test_setup(VoteAckMessage_receive_propagates_to_all_when_current_node_is_not_target, test_setup),
          cmocka_unit_test_setup(VoteAckMessage_receive_does_not_propagate_when_current_node_is_target, test_setup),
          cmocka_unit_test_setup(VoteAckMessage_receive_deletes_correct_vote_from_storage_when_timestamp_matches, test_setup),
          cmocka_unit_test_setup(VoteAckMessage_receive_does_not_delete_vote_from_storage_when_timestamps_do_not_match, test_setup),
          cmocka_unit_test_setup(VoteAckMessage_receive_broadcasts_next_vote_when_current_node_is_target, test_setup),
          cmocka_unit_test_setup(VoteAckMessage_broadcast_sends_to_all_connections, test_setup),
  };

  return cmocka_run_group_tests_name("VoteAckMessageTest", tests, NULL, NULL);
}

int main() {
  return RunVoteAckMessageTest();
}
