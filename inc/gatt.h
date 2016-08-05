#pragma once

#include <ble.h>
#include <ble_gap.h>

#include <connection.h>
#include <error.h>
#include <logger.h>
#include <message.h>

// FOO_SERVICE_UUID: 12ec7140-cec6-11e5-b3ae-0002a5d5c51b
#define FOO_SERVICE_UUID               0x1B, 0xC5, 0xD5, 0xA5, 0x02, 0x00, 0xAE, 0xB3, 0xE5, 0x11, 0xC6, 0xCE, 0x40, 0x71, 0xEC, 0x12
#define FOO_CHARACTERISTIC_UUID        0x1524
#define FOO_CHARACTERISTIC_MAX_LENGTH  40
#define FOO_CHARACTERISTIC_DESCRIPTION "meshData"

typedef MessagePropagationType (*write_event_handler)(uint16_t connectionHandle, uint8_t *dataPacket);

void gatt_initialize(void);
void initialize_write_events(void);
void broadcast_logging_message(char **parsedCommandArray);
void propagate_message(ble_gatts_evt_t *gattsEvent);
void send_message_from_connection_queue(connection *targetConnection);
void send_to_single_connection(connection *targetConnection, uint8_t *data, uint16_t dataLength);
void send_to_central_connection(uint16_t originHandle, uint8_t *data, uint16_t dataLength);
void send_to_all_connections(uint16_t originHandle, uint8_t *data, uint16_t dataLength);
void scheduled_broadcast_request(void *data, uint16_t dataLength);
void handle_write_event(void * data, uint16_t dataLength);
