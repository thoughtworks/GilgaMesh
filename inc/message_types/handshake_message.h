#pragma once

#include "message.h"
#include "connection.h"

typedef struct {
  BleMessageHead head;
  uint32_t deviceId;
  uint8_t majorVersion;
  uint8_t minorVersion;
}__attribute__ ((packed)) BleMessageHandshakeReq;


MessagePropagationType receive_handshake_message(uint16_t connectionHandle, uint8_t *dataPacket);
void send_handshake_message(connection *targetConnection);