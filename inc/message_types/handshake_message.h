#pragma once

#include "message.h"
#include "connection.h"

typedef struct {
  BleMessageType messageType;
  uint32_t deviceId;
  uint8_t majorVersion;
  uint8_t minorVersion;
}__attribute__ ((packed)) BleMessageHandshakeReq;


void handshake_message_initialize(void);
void send_handshake_message(connection *targetConnection);
MessagePropagationType receive_handshake_message(uint16_t connectionHandle, uint8_t *dataPacket);
