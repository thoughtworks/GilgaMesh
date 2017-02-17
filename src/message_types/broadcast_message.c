#include "message_types/broadcast_message.h"
#include "system/log.h"
#include "gatt.h"

static BleMessageType broadcastMessageType;

bool broadcast_message_initialize() {
  broadcastMessageType = register_message_type(receive_broadcast_message);

  return true;
}

void send_broadcast_message(char **parsedCommandArray, uint8_t numCommands) {
  UNUSED_PARAMETER(numCommands);

  if (strlen(parsedCommandArray[1]) > BROADCAST_MESSAGE_MAX_SIZE) {
    MESH_LOG("ERROR: Broadcast message longer than %u characters\r\n", BROADCAST_MESSAGE_MAX_SIZE);
    return;
  }

  BleMessageBroadcastReq request;
  memset(&request, 0, sizeof(request));
  request.messageType = broadcastMessageType;
  memcpy(request.message, parsedCommandArray[1], strlen(parsedCommandArray[1])+1);

  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(BleMessageBroadcastReq));
}

MessagePropagationType receive_broadcast_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  BleMessageBroadcastReq *broadcastMessage = (BleMessageBroadcastReq *) dataPacket;

  MESH_LOG("***** RECEIVED Broadcast message: %s\r\n", broadcastMessage->message);

  return PropagateToAll;
}