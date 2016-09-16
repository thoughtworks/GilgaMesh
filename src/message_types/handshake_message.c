#include "message_types/handshake_message.h"
#include "device.h"
#include "gatt.h"
#include "version.h"

static BleMessageType handshakeMessageType;

void handshake_message_initialize() {
  handshakeMessageType = register_message_type(receive_handshake_message);
}

void send_handshake_message(connection *targetConnection) {
  BleMessageHandshakeReq request;
  memset(&request, 0, sizeof(request));
  request.messageType = handshakeMessageType;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = get_raw_device_id();

  send_to_single_connection(targetConnection, (uint8_t *)&request, sizeof(request));
}


MessagePropagationType receive_handshake_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  BleMessageHandshakeReq *msg = (BleMessageHandshakeReq *)dataPacket;
  connection *conn = find_active_connection_by_handle(connectionHandle);
  if (conn != NULL) {
    conn->majorVersion = msg->majorVersion;
    conn->minorVersion = msg->minorVersion;
    conn->deviceId = msg->deviceId;
  }
  return DoNotPropagate;
}