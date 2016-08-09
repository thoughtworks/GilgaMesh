#include "message_types/broadcast_message.h"
#include "gatt.h"


void send_broadcast_message(char **parsedCommandArray) {
  if (strlen(parsedCommandArray[0]) > BROADCAST_MESSAGE_MAX_SIZE) {
    NRF_LOG_PRINTF("ERROR: Broadcast message longer than %u characters\r\n", BROADCAST_MESSAGE_MAX_SIZE);
    return;
  }

  BleMessageBroadcastReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Broadcast;
  memcpy(request.message, parsedCommandArray[0], BROADCAST_MESSAGE_MAX_SIZE);

  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(BleMessageBroadcastReq));
}

MessagePropagationType receive_broadcast_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  BleMessageBroadcastReq *msg = (BleMessageBroadcastReq *) dataPacket;

  char content[BROADCAST_MESSAGE_MAX_SIZE + 1];
  content[BROADCAST_MESSAGE_MAX_SIZE] = 0; // append string termination
  for (int i = 0 ; i < BROADCAST_MESSAGE_MAX_SIZE ; i++) {
    content[i] = msg->message[i];
  }

  NRF_LOG_PRINTF("***** RECEIVED Broadcast message: %s\r\n", content);

  return PropagateToAll;
}