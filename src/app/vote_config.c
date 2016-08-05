#include <votes.h>
#include <sdk_common.h>
#include <gatt.h>
#include <storage.h>
#include <app_scheduler.h>
#include <feedback.h>


static voteConfiguration voteConfig = { 1, 1 };


void update_voting_group(uint8_t newGroup) {
  if (newGroup != voteConfig.group) {
    voteConfig.group = newGroup;
    display_group_value_change_feedback();
    save_vote_configuration();
  }
}

void update_voting_value(uint8_t newValue) {
  if (newValue != voteConfig.value) {
    voteConfig.value = newValue;
    display_group_value_change_feedback();
    save_vote_configuration();
  }
}

voteConfiguration *get_vote_configuration() {
  return &voteConfig;
}

void set_vote_configuration(uint8_t *data) {
  update_voting_group(data[0]);
  update_voting_value(data[1]);
}

void broadcast_group_value_update(char* nodeIdStr, char* newFieldStr, bool isGroup) {
  BleMessageGroupValueReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = SetVoteConfiguration;
  request.deviceId = (uint32_t) atoll(nodeIdStr);
  if (isGroup) {
    request.setGroup = true;
    request.newGroup = (uint8_t) atoi(newFieldStr);
  } else {
    request.setValue = true;
    request.newValue = (uint8_t) atoi(newFieldStr);
  }

  app_sched_event_put(&request, sizeof(BleMessageGroupValueReq), scheduled_broadcast_request);
}

MessagePropagationType receive_group_value_update(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  BleMessageGroupValueReq *req = (BleMessageGroupValueReq *) dataPacket;
  if (req->deviceId == deviceId) {
    if (req->setGroup) update_voting_group(req->newGroup);
    if (req->setValue) update_voting_value(req->newValue);
    return DoNotPropagate;
  }
  return PropagateToAll;
}
