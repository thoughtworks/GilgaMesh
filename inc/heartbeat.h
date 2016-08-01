#pragma once

#include "message.h"
#include "queue.h"

void broadcast_heartbeat(void *data, uint16_t dataLength);
void receive_heartbeat(uint8_t *heartbeat);
