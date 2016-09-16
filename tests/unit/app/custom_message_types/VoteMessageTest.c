#include "app/custom_message_types/vote_message.h"

#include "cmocka_includes.h"
#include <ble_types.h>

static BleMessageVoteReq mockRequest;
static BleMessageType mockMessageType = 123;
static uint32_t deviceId;
userVote mockVote;

static int test_setup(void **state) {
  deviceId = get_raw_device_id();
  memset(&mockRequest, 0, sizeof(mockRequest));
  mockRequest.messageType = mockMessageType;
  mockRequest.deviceId = deviceId;

  return 0;
}

static void VoteMessage_initialize_registers_message_and_starts_timer() {
  will_return(register_message_type, mockMessageType);

  expect_any(create_single_shot_timer, timer_id);
  expect_value(start_timer, ms_to_execute, VOTE_MESSAGE_FREQUENCY_IN_MS);
  expect_any(start_timer, timer_id);

  vote_message_initialize();
}

static void VoteMessage_receive_propagates_to_central_connection() {
  assert_int_equal(PropagateToCentral, receive_vote_message(0, &mockRequest));
}

static void VoteMessage_send_should_do_something_TEST_THIS() {
  assert_true(true);
}

int RunVoteMessageTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test_setup(VoteMessage_initialize_registers_message_and_starts_timer, test_setup),
          cmocka_unit_test_setup(VoteMessage_receive_propagates_to_central_connection, test_setup),
          cmocka_unit_test_setup(VoteMessage_send_should_do_something_TEST_THIS, test_setup),
  };

  return cmocka_run_group_tests_name("VoteMessageTest", tests, NULL, NULL);
}

int main() {
  return RunVoteMessageTest();
}
