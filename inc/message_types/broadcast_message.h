#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "message.h"

#define BROADCAST_MESSAGE_MAX_SIZE  12

typedef struct {
  BleMessageType messageType;
  uint8_t message[BROADCAST_MESSAGE_MAX_SIZE + 1];
}__attribute__ ((packed)) BleMessageBroadcastReq;


bool broadcast_message_initialize(void);
void send_broadcast_message(char **parsedCommandArray, uint8_t numCommands);
MessagePropagationType receive_broadcast_message(uint16_t connectionHandle, uint8_t *dataPacket);
