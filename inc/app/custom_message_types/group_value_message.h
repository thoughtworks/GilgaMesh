#pragma once

#include "app/vote_config.h"
#include "message_types/message.h"

typedef struct {
  BleMessageHead head;
  uint32_t deviceId;
  bool setGroup;
  uint8_t newGroup;
  bool setValue;
  uint8_t newValue;
}__attribute__ ((packed)) BleMessageGroupValueReq;

void group_value_message_initialize(void);
void broadcast_group_value_message(char **parsedCommandArray, uint8_t numCommands);
MessagePropagationType receive_group_value_message(uint16_t connectionHandle, uint8_t *dataPacket);
