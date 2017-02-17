#pragma once

#include <stdint.h>

typedef enum {
  DoNotPropagate,
  PropagateToAll,
  PropagateToCentral
} MessagePropagationType;

typedef uint8_t BleMessageType;  // Message Type is used to identify a request

