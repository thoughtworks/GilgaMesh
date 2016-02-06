#pragma once

#include <stdbool.h>

#include <ble_gap.h>

#include <logger.h>

#define ATTR_MAX_PERIPHERAL_CONNS      3

typedef enum {CENTRAL, PERIPHERAL} ConnectionType;

typedef struct
{
  bool active;
  uint16_t connectionHandle;
  ConnectionType type;
  ble_gap_addr_t address;
} connection;

typedef struct
{
  connection central;
  connection peripheral[ATTR_MAX_PERIPHERAL_CONNS];
} connections;


connections *activeConnections;

void connections_initialize(void);
void set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress);
void set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress);
void unset_connection(uint16_t connectionHandle);
void print_all_connections(void);
