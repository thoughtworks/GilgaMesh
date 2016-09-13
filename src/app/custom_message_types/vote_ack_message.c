#include "app/custom_message_types/vote_ack_message.h"

#include <string.h>
#include <stdlib.h>
#include <app_scheduler.h>
#include <system/log.h>

#include "device.h"
#include "gatt.h"


void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr) {
  BleMessageVoteAckReq request;

  memset(&request, 0, sizeof(request));
  request.head.messageType = 6; // 6 == VoteAck
  request.deviceId = (uint32_t) atoll(nodeIdStr);
  request.voteAck.voterId = (uint16_t) atoi(voterIdStr);
  rtc_timestring_to_timestamp(timestampStr, request.voteAck.timestamp);

  app_sched_event_put(&request, sizeof(BleMessageVoteAckReq), scheduled_broadcast_request);
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
    MESH_LOG("recieved vote ack!\r\n");
    //remove this vote from fstorage
    //send out next vote to gateway:
    //get_random_vote(NULL, 0);
    return DoNotPropagate;
  }
  return PropagateToAll;
}