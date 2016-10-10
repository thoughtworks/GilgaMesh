#include "app/custom_message_types/vote_ack_message.h"

#include <string.h>
#include <stdlib.h>

#include "app/feedback.h"
#include "app/storage.h"
#include "app/custom_message_types/vote_message.h"
#include "command.h"
#include "device.h"
#include "gatt.h"

static BleMessageType voteAckMessageType;

bool vote_ack_message_initialize() {
  voteAckMessageType = register_message_type(receive_vote_acknowledgement);

  mesh_add_terminal_command("vack", "Broadcast vote ack", broadcast_vote_ack_from_cmd_line);

  return true;
}

static void delete_vote_and_send_next_vote(userVoteAck *voteAck) {
  userVote matchingVote;
  storageOperationResult result = get_data_from_storage(VOTES_STORAGE_FILE_ID, voteAck->voterId, &matchingVote, sizeof(matchingVote));
  if (result == SUCCESS && rtc_is_equal_timestamp(voteAck->timestamp, matchingVote.timeOfLastVote)) {
    result = delete_data_from_storage(VOTES_STORAGE_FILE_ID, voteAck->voterId);
  }
  if (result == FAILURE) {
    display_failure_feedback();
  }
  send_vote_message();
}

void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr) {
  BleMessageVoteAckReq request;

  memset(&request, 0, sizeof(request));
  request.messageType = voteAckMessageType;
  request.deviceId = (uint32_t) atoll(nodeIdStr);
  request.voteAck.voterId = (uint16_t) atoi(voterIdStr);
  rtc_timestring_to_timestamp(timestampStr, request.voteAck.timestamp);

  receive_vote_acknowledgement(NULL, (uint8_t *) &request);
  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(request));
}

void broadcast_vote_ack_from_cmd_line(char** parsedCommandArray, uint8_t numCommands) {
  UNUSED_PARAMETER(numCommands);
  broadcast_vote_acknowledgement(parsedCommandArray[1], parsedCommandArray[2], parsedCommandArray[3]);
}

MessagePropagationType receive_vote_acknowledgement(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  BleMessageVoteAckReq *voteAckReq = (BleMessageVoteAckReq *) dataPacket;

  if (voteAckReq->deviceId == get_raw_device_id()) {
    delete_vote_and_send_next_vote(&voteAckReq->voteAck);
    return DoNotPropagate;
  }

  return PropagateToAll;
}
