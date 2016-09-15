#include "app/custom_message_types/vote_message.h"

#include <sdk_common.h>
#include <stdlib.h>

#include "app/storage.h"
#include "system/log.h"
#include "system/timer.h"
#include "device.h"
#include "gatt.h"

#define MS_RATE_TO_SEND_VOTE 5000

SYS_TIMER_DEF(voteMessageTimer);

void vote_message_initialize() {
  add_write_event(5, receive_vote);

  create_single_shot_timer(&voteMessageTimer);
  start_timer(&voteMessageTimer, MS_RATE_TO_SEND_VOTE, broadcast_next_vote);
}

static void log_vote(BleMessageVoteReq *request) {
  timestring_t timestring;

  char votingNodeName[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->deviceId, votingNodeName);

  rtc_timestamp_to_timestring(request->vote.timeOfLastVote, timestring);

  char voteLogInfo[200];
  sprintf(voteLogInfo, "VOTE: {\"id\": \"%s\", \"rawId\": %u, \"voterId\": \"%u\", \"hitCount\": %u, \"group\": %u, \"value\": %u, \"timestamp\": \"%s\"}",
          votingNodeName, (unsigned int) request->deviceId, request->vote.voterId, request->vote.hitCount, request->vote.config.group,
          request->vote.config.value, timestring);
  MESH_LOG("%s\r\n", voteLogInfo);
}

void broadcast_next_vote() {
  stop_timer(&voteMessageTimer);
  userVote *voteToSend = get_data_from_storage(VOTES_STORAGE_FILE_ID, NULL);

  if (voteToSend != NULL) {
    BleMessageVoteReq request;
    memset(&request, 0, sizeof(request));
    request.messageType = 5; // 5 == Vote
    request.deviceId = get_raw_device_id();
    request.vote = *voteToSend;

    log_vote(&request);
    send_to_central_connection(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(BleMessageVoteReq));
  }

  start_timer(&voteMessageTimer, MS_RATE_TO_SEND_VOTE, broadcast_next_vote);
}

MessagePropagationType receive_vote(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  log_vote((BleMessageVoteReq *)dataPacket);
  return PropagateToCentral;
}
