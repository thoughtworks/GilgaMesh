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


void gatt_initialize(void);
void propagate_family_id(uint16_t originHandle);
void broadcast_message(char* message);
void broadcast_heartbeat(void *data, uint16_t dataLength);
void broadcast_vote(unsigned short voterId);
void log_and_propagate_heartbeat(uint16_t originHandle, BleMessageHeartbeatReq *heartbeat);
void log_and_propagate_vote(uint16_t originHandle, BleMessageVoteReq *request);
void share_connection_info(connection *targetConnection);
void retry_send_to_single_connection(connection *targetConnection, uint8_t messageCount);
