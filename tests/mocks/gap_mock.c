#include <gap.h>

#include "cmocka_includes.h"

#if defined(TESTING) && !defined(GAP_TEST)

void gap_initialize(void) {
}

void set_advertising_data(void) {
}

void start_advertising(void) {
}

void start_scanning(void) {
}

void check_advertising_status(char **parsedCommandArray) {
}

void check_scanning_status(char **parsedCommandArray) {
}

void disconnect_from_peer(uint16_t connectionHandle) {
}

void handle_gap_event(ble_evt_t * bleEvent) {
}

void receive_broadcast_message(uint8_t *request) {
}

#endif