#include <gatt.h>

#include "cmocka_includes.h"

#if defined(TESTING) && !defined(GATT_TEST)

void broadcast_logging_message(char **parsedCommandArray) {
  check_expected_ptr(parsedCommandArray);
}

void gatt_initialize(void) { }
void propagate_message(ble_gatts_evt_t *gattsEvent) { }
void send_message_from_connection_queue(connection *targetConnection){ }
void send_to_single_connection(connection *targetConnection, uint8_t *data, uint16_t dataLength) { }
void send_to_central_connection(uint16_t originHandle, uint8_t *data, uint16_t dataLength) { }
void send_to_all_connections(uint16_t originHandle, uint8_t *data, uint16_t dataLength) { }
void scheduled_broadcast_request(void *data, uint16_t dataLength) { }
void handle_write_event(void * data, uint16_t dataLength) { }

#endif