#pragma once

#if defined(TESTING) && !defined(CONNECTION_TEST)
#include "connection_mock.h"

#else
#include <stdbool.h>

#include <ble_gap.h>
#include <nrf_soc.h>

#include <logger.h>

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
} connection;

typedef struct
{
  connection central;
  connection peripheral[ATTR_MAX_PERIPHERAL_CONNS];
} connections;


uint16_t nodeId;
uint32_t familyId;
connections *activeConnections;

void connections_initialize(void);
void set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress);
void set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress);
ConnectionType unset_connection(uint16_t connectionHandle);
void print_all_connections(void);
connection* find_active_connection_by_address(ble_gap_addr_t address);
bool central_connection_active(void);
bool peripheral_connections_active(void);

#endif
