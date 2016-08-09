#pragma once

#include <stdbool.h>

#include <ble_gap.h>
#include <nrf_soc.h>

#include "conversion.h"
#include "logger.h"
#include "message_types/message.h"
#include "queue.h"

#define ATTR_MAX_CENTRAL_CONNS         1
#define ATTR_MAX_PERIPHERAL_CONNS      3
#define ATTR_MAX_CONNECTIONS           ATTR_MAX_CENTRAL_CONNS + ATTR_MAX_PERIPHERAL_CONNS

typedef enum {INVALID, CENTRAL, PERIPHERAL} ConnectionType;

typedef struct
{
    bool active;
    uint16_t handle;
    ConnectionType type;
    ble_gap_addr_t address;
    uint32_t deviceId;
    uint8_t majorVersion;
    uint8_t minorVersion;
    transmissionPacketQueue unsentMessages;
} connection;

typedef struct
{
    connection central;
    connection peripheral[ATTR_MAX_PERIPHERAL_CONNS];
} connections;

uint32_t deviceId;
char *nodeName;
connections *activeConnections;

void connections_initialize(void);
connection* set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress);
connection* set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress);
ConnectionType unset_connection(uint16_t connectionHandle);
void print_all_connections(char **parsedCommandArray);
connection* find_active_connection_by_handle(uint16_t connectionHandle);
bool central_connection_active(void);
bool peripheral_connections_active(void);
bool all_peripheral_connections_active(void);
char* get_connection_info(connection *conn, char* result);
