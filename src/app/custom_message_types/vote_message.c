#include "app/custom_message_types/vote_message.h"

#include <sdk_common.h>
#include <stdlib.h>
#include <app/feedback.h>

#include "app/storage.h"
#include "app/uart.h"
#include "system/log.h"
#include "system/timer.h"
#include "device.h"
#include "gatt.h"

SYS_TIMER_DEF(voteMessageTimer);

static BleMessageType voteMessageType;

void vote_message_initialize() {
  voteMessageType = register_message_type(receive_vote_message);

  create_single_shot_timer(&voteMessageTimer);
  start_timer(&voteMessageTimer, VOTE_MESSAGE_FREQUENCY_IN_MS, send_vote_message);
}

static void log_vote(BleMessageVoteReq *request) {
  timestring_t timestring;

  char votingNodeName[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->deviceId, votingNodeName);

  rtc_timestamp_to_timestring(request->vote.timeOfLastVote, timestring);

  char voteLogInfo[200];
  sprintf(voteLogInfo, "VOTE: {\"id\": \"%s\", \"rawId\": %u, \"voterId\": \"%u\", \"hitCount\": %u, \"group\": %u, \"value\": %u, \"timestamp\": \"%s\"}\r\n",
          votingNodeName, (unsigned int) request->deviceId, request->vote.voterId, request->vote.hitCount, request->vote.config.group,
          request->vote.config.value, timestring);
  MESH_LOG("%s", voteLogInfo);
  log_to_uart(voteLogInfo, sizeof(voteLogInfo));

}

void send_vote_message() {
  stop_timer(&voteMessageTimer);
  userVote voteToSend;
  storageOperationResult result = get_data_from_storage(VOTES_STORAGE_FILE_ID, NULL, &voteToSend, sizeof(voteToSend));

  if (result == SUCCESS) {
    BleMessageVoteReq request;
    memset(&request, 0, sizeof(request));
    request.messageType = voteMessageType;
    request.deviceId = get_raw_device_id();
    request.vote = voteToSend;

    log_vote(&request);
    send_to_central_connection(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(BleMessageVoteReq));

  } else if (result == FAILURE) {
    display_failure_feedback();
  }

  start_timer(&voteMessageTimer, VOTE_MESSAGE_FREQUENCY_IN_MS, send_vote_message);
}

MessagePropagationType receive_vote_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  log_vote((BleMessageVoteReq *)dataPacket);
  return PropagateToCentral;
}
