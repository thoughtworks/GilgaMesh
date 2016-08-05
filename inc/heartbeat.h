#pragma once

#include "message.h"
#include "queue.h"

void broadcast_heartbeat(void *data, uint16_t dataLength);
MessagePropagationType receive_heartbeat(uint16_t connectionHandle, uint8_t *dataPacket);
