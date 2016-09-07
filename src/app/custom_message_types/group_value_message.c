#include "app/custom_message_types/group_value_message.h"

#include <app_scheduler.h>
#include <stdlib.h>
#include "device.h"
#include "gatt.h"

void (*mesh_command_handler)(char **parsedCommandArray, uint8_t numCommands);

void broadcast_group_value_update(char **parsedCommandArray, uint8_t numCommands) {
  UNUSED_PARAMETER(numCommands);

  BleMessageGroupValueReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Custom;
  request.deviceId = (uint32_t) atoll(parsedCommandArray[1]);

  bool isGroupUpdate = (strcmp(parsedCommandArray[0], "grp") == 0) ? true : false;
  if (isGroupUpdate) {
    request.setGroup = true;
    request.newGroup = (uint8_t) atoi(parsedCommandArray[2]);
  } else {
    request.setValue = true;
    request.newValue = (uint8_t) atoi(parsedCommandArray[2]);
  }

  app_sched_event_put(&request, sizeof(BleMessageGroupValueReq), scheduled_broadcast_request);
}

MessagePropagationType receive_group_value_update(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  BleMessageGroupValueReq *req = (BleMessageGroupValueReq *) dataPacket;
  if (req->deviceId == get_raw_device_id()) {
    if (req->setGroup) update_voting_group(req->newGroup);
    if (req->setValue) update_voting_value(req->newValue);
    return DoNotPropagate;
  }
  return PropagateToAll;
}