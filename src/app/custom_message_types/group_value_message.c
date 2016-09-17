#include "app/custom_message_types/group_value_message.h"

#include <stdlib.h>

#include "system/util.h"
#include "command.h"
#include "device.h"
#include "gatt.h"

static BleMessageType groupValueMessageType;

void group_value_message_initialize() {
  groupValueMessageType = register_message_type(receive_group_value_message);

  mesh_add_terminal_command("grp", "Update group", send_group_value_message);
  mesh_add_terminal_command("val", "Update value", send_group_value_message);
}

void send_group_value_message(char **parsedCommandArray, uint8_t numCommands) {
  SYS_UNUSED_PARAMETER(numCommands);

  BleMessageGroupValueReq request = { 0 };
  request.messageType = groupValueMessageType;
  request.deviceId = (uint32_t) atoll(parsedCommandArray[1]);

  bool isGroupUpdate = (strcmp(parsedCommandArray[0], "grp") == 0) ? true : false;
  if (isGroupUpdate) {
    request.setGroup = true;
    request.newGroup = (uint8_t) atoi(parsedCommandArray[2]);
  } else {
    request.setValue = true;
    request.newValue = (uint8_t) atoi(parsedCommandArray[2]);
  }

  receive_group_value_message(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request);
  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(request));
}

MessagePropagationType receive_group_value_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  SYS_UNUSED_PARAMETER(connectionHandle);
  BleMessageGroupValueReq *req = (BleMessageGroupValueReq *) dataPacket;
  if (req->deviceId == get_raw_device_id()) {
    if (req->setGroup) update_voting_group(req->newGroup);
    if (req->setValue) update_voting_value(req->newValue);
    return DoNotPropagate;
  }
  return PropagateToAll;
}