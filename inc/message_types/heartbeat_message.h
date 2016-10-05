#pragma once

#include "message.h"
#include "queue.h"

#define HEARTBEAT_MESSAGE_FREQUENCY_IN_MS 3000

typedef struct {
  BleMessageType messageType;
  uint32_t deviceId;
  uint32_t centralConnectionDeviceId;
  uint8_t majorVersion;
  uint8_t minorVersion;
}__attribute__ ((packed)) BleMessageHeartbeatReq;


bool heartbeat_message_initialize(void);
void send_heartbeat_message(void);
MessagePropagationType receive_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket);
