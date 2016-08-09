#pragma once

#include "message.h"
#include "queue.h"

typedef struct {
  BleMessageHead head;
  uint32_t deviceId;
  uint32_t centralConnectionDeviceId;
  uint8_t majorVersion;
  uint8_t minorVersion;
  uint16_t voteCount;
  nfcStatus nfc;
}__attribute__ ((packed)) BleMessageHeartbeatReq;


void send_heartbeat_message(void *data, uint16_t dataLength);
MessagePropagationType receive_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket);
