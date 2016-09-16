#include "app/custom_message_types/vote_ack_message.h"
#include "cmocka_includes.h"

void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr) { }
void broadcast_vote_ack_from_cmd_line(char** parsedCommandArray, uint8_t numCommands) { }
MessagePropagationType receive_vote_acknowledgement(uint16_t connectionHandle, uint8_t *dataPacket) {
  return PropagateToAll;
}
