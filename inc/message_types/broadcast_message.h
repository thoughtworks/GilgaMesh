#pragma once

#include "message.h"

#define BROADCAST_MESSAGE_MAX_SIZE  12

typedef struct {
  BleMessageHead head;
  uint8_t message[BROADCAST_MESSAGE_MAX_SIZE];
}__attribute__ ((packed)) BleMessageBroadcastReq;


void send_broadcast_message(char **parsedCommandArray);
MessagePropagationType receive_broadcast_message(uint16_t connectionHandle, uint8_t *dataPacket);