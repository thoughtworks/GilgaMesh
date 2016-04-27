#include <queue.h>

#include "cmocka_includes.h"


bool push_onto_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t dataLength) {
  check_expected_ptr(queue);
  return mock_type(bool);
}

bool pop_from_queue(transmissionPacketQueue *queue, uint8_t *data, uint16_t *dataLength) {
  check_expected_ptr(queue);
  return mock_type(bool);
}

bool queue_is_empty(transmissionPacketQueue *queue) {
  check_expected_ptr(queue);
  return mock_type(bool);
}
