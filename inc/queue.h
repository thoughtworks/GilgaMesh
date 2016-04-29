#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_QUEUE_DATA_LENGTH          23
#define MAX_QUEUE_SIZE                 20


typedef enum
{
  MESSAGE_PRIORITY_HIGH,
  MESSAGE_PRIORITY_NORMAL
} messagePriority;

typedef struct
{
  uint16_t dataLength;
  uint8_t data[MAX_QUEUE_DATA_LENGTH];
}__attribute__ ((packed)) transmissionPacket;

typedef struct
{
  uint8_t count;
  uint8_t front;
  uint8_t normalPriorityFront;
  transmissionPacket queue[MAX_QUEUE_SIZE];
}__attribute__ ((packed)) transmissionPacketQueue;


bool push_onto_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t dataLength, messagePriority priority);
bool peek_from_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t *dataLength);
bool pop_from_queue(transmissionPacketQueue *queue);
bool queue_is_empty(transmissionPacketQueue *queue);
