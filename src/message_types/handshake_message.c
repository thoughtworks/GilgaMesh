#include "message_types/handshake_message.h"
#include "gatt.h"
#include "version.h"

void send_handshake_message(connection *targetConnection) {
  BleMessageHandshakeReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Handshake;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = deviceId;

  send_to_single_connection(targetConnection, (uint8_t *)&request, sizeof(request));
}


MessagePropagationType receive_handshake_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  BleMessageHandshakeReq *msg = (BleMessageHandshakeReq *)dataPacket;
  connection *conn = find_active_connection_by_handle(connectionHandle);
  if (conn != 0) {
    conn->majorVersion = msg->majorVersion;
    conn->minorVersion = msg->minorVersion;
    conn->deviceId = msg->deviceId;
  }
  return DoNotPropagate;
}