#include <queue.h>
#include <string.h>

bool queue_is_empty(transmissionPacketQueue *queue)
{
  return queue->count <= 0;
}

bool queue_is_full(transmissionPacketQueue *queue)
{
  return queue->count >= MAX_QUEUE_SIZE;
}

transmissionPacket create_packet(uint8_t *data, uint16_t dataLength)
{
  transmissionPacket packet;
  packet.dataLength = dataLength;
  memcpy(packet.data, data, dataLength);
  return packet;
}

bool push_onto_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t dataLength)
{
  if (queue_is_full(queue)) return false;
  if (dataLength > MAX_QUEUE_DATA_LENGTH) return false;

  uint8_t insertIndex = (uint8_t) ((queue->front + queue->count) % MAX_QUEUE_SIZE);
  queue->queue[insertIndex] = create_packet(data, dataLength);
  queue->count += 1;
  return true;
}

bool pop_from_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t *dataLength) {
  if (queue_is_empty(queue)) return false;

  transmissionPacket frontPacket = queue->queue[queue->front];
  memcpy(data, frontPacket.data, frontPacket.dataLength);
  memcpy(dataLength, &frontPacket.dataLength, sizeof(uint16_t));

  queue->front = (uint8_t) ((queue->front + 1) % MAX_QUEUE_SIZE);
  queue->count -= 1;
  return true;
}