#pragma once

#include <stdbool.h>

#include <ble_gap.h>
#include <nrf_soc.h>

#include "conversion.h"
#include "logger.h"
#include "message_types/message.h"
#include "queue.h"

#define MAX_CENTRAL_CONNECTIONS        1
#define MAX_PERIPHERAL_CONNECTIONS     3
#define MAX_TOTAL_CONNECTIONS          MAX_CENTRAL_CONNECTIONS + MAX_PERIPHERAL_CONNECTIONS

typedef enum {INVALID, CENTRAL, PERIPHERAL} ConnectionType;

typedef struct
{
    bool active;
    uint16_t handle;
    ConnectionType type;
    uint32_t deviceId;
    uint8_t majorVersion;
    uint8_t minorVersion;
    transmissionPacketQueue unsentMessages;
}__attribute__ ((packed)) connection;

char *nodeName;

void connections_initialize(void);
connection* set_connection(uint16_t connectionHandle, ConnectionType connectionType);
ConnectionType unset_connection(uint16_t connectionHandle);
void print_all_connections(char **parsedCommandArray);
connection* find_active_connection_by_handle(uint16_t connectionHandle);
bool central_connection_active(void);
bool all_peripheral_connections_active(void);
bool is_connection_active(uint16_t connectionHandle);
uint32_t get_central_connection_device_id(void);
void print_single_connection_info(uint16_t connectionHandle);