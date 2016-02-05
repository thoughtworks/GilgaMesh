#pragma once

#include <stdbool.h>

#include <ble_gap.h>

#include <logger.h>

#define ATTR_MAX_PERIPHERAL_CONNS      3

typedef struct
{
  bool exists;
  ble_gap_addr_t address;
} connection;

typedef struct
{
  connection central;
  connection peripheral[ATTR_MAX_PERIPHERAL_CONNS];
} connections;


void connections_initialize();
void set_central_connection(ble_gap_addr_t deviceAddress);
void set_peripheral_connection(ble_gap_addr_t deviceAddress);
