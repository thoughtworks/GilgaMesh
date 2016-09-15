#include "app/custom_message_types/vote_ack_message.h"

#include <string.h>
#include <stdlib.h>
#include <app/storage.h>
#include <app/custom_message_types/vote_message.h>

#include "device.h"
#include "gatt.h"


void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr) {
  BleMessageVoteAckReq request;

  memset(&request, 0, sizeof(request));
  request.head.messageType = 6; // 6 == VoteAck
  request.deviceId = (uint32_t) atoll(nodeIdStr);
  request.voteAck.voterId = (uint16_t) atoi(voterIdStr);
  rtc_timestring_to_timestamp(timestampStr, request.voteAck.timestamp);

  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *)&request, sizeof(BleMessageVoteAckReq));
  receive_vote_acknowledgement(NULL, &request);
}

void broadcast_vote_ack_from_cmd_line(char** parsedCommandArray, uint8_t numCommands) {
  UNUSED_PARAMETER(numCommands);

  broadcast_vote_acknowledgement(parsedCommandArray[1], parsedCommandArray[2], parsedCommandArray[3]);
}

MessagePropagationType receive_vote_acknowledgement(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  BleMessageVoteAckReq *voteAckReq = (BleMessageVoteAckReq *) dataPacket;
  if (voteAckReq->deviceId == get_raw_device_id()) {
    userVoteAck voteAck = voteAckReq->voteAck;
    userVote *matchingVote = get_data_from_storage(VOTES_STORAGE_FILE_ID, voteAck.voterId);

    if (matchingVote != NULL && rtc_is_equal_timestamp(voteAck.timestamp, matchingVote->timeOfLastVote)) {
      delete_data_from_storage(VOTES_STORAGE_FILE_ID, voteAck.voterId);
    }
    broadcast_next_vote();
    return DoNotPropagate;
  }
  return PropagateToAll;
}