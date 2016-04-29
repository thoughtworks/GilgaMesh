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


uint8_t first_empty_index(transmissionPacketQueue *queue)
{
  return (uint8_t) ((queue->front + queue->count) % MAX_QUEUE_SIZE);
}


void increment_index(uint8_t *queueIndex)
{
  memset(queueIndex, ((*queueIndex + 1) % MAX_QUEUE_SIZE), sizeof(uint8_t));
}


void shift_normal_priority_messages_down(transmissionPacketQueue *queue)
{
  int index = first_empty_index(queue);
  while (index != queue->normalPriorityFront) {
    int oldIndex = ((index - 1) + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    queue->queue[index] = queue->queue[oldIndex];
    index = oldIndex;
  }
  increment_index(&queue->normalPriorityFront);
}


bool push_onto_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t dataLength, messagePriority priority)
{
  if (queue_is_full(queue)) return false;
  if (dataLength > MAX_QUEUE_DATA_LENGTH) return false;

  uint8_t insertIndex = first_empty_index(queue);
  if (priority == MESSAGE_PRIORITY_HIGH) {
    insertIndex = queue->normalPriorityFront;
    shift_normal_priority_messages_down(queue);
  }
  queue->queue[insertIndex] = create_packet(data, dataLength);
  queue->count += 1;
  return true;
}


bool peek_from_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t *dataLength) {
  if (queue_is_empty(queue)) return false;

  transmissionPacket frontPacket = queue->queue[queue->front];
  memcpy(data, frontPacket.data, frontPacket.dataLength);
  memcpy(dataLength, &frontPacket.dataLength, sizeof(uint16_t));

  return true;
}


bool pop_from_queue(transmissionPacketQueue *queue) {
  if (queue_is_empty(queue)) return false;

  if (queue->front == queue->normalPriorityFront) {
    increment_index(&queue->normalPriorityFront);
  }
  increment_index(&queue->front);
  queue->count -= 1;
  return true;
}
