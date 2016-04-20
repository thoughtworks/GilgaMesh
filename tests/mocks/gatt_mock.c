#include <gatt.h>

#include "cmocka_includes.h"

#if defined(TESTING) && !defined(GATT_TEST)

void gatt_initialize(void) {

}

void propagate_family_id(uint16_t originHandle) {
  check_expected(originHandle);
}

void broadcast_message(char* message) {
  check_expected_ptr(message);
}

void broadcast_vote(unsigned short voterId) {
  check_expected(voterId);
}

void log_and_propagate_heartbeat(uint16_t originHandle, BleMessageHeartbeatReq *heartbeat) {
  check_expected(originHandle);
}

void log_and_propagate_vote(uint16_t originHandle, BleMessageVoteReq *request) {
  check_expected(originHandle);
}

void share_connection_info(uint16_t targetHandle) {
  check_expected(targetHandle);
}


#endif