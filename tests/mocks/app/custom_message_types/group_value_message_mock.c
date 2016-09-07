#include "app/custom_message_types/group_value_message.h"

#include <cmocka_includes.h>

void broadcast_group_value_update(char **parsedCommandArray, uint8_t numCommands) { }
MessagePropagationType receive_group_value_update(uint16_t connectionHandle, uint8_t *dataPacket) {
  return DoNotPropagate;
}
