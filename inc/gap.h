#pragma once

#include <string.h>

#include <nrf_delay.h>
#include <app_util.h>
#include <app_error.h>
#include <ble.h>
#include <ble_gap.h>

#include <logger.h>
#include <connection.h>

#define MESH_NAME                      "MeshyMesh"
#define MESH_NAME_SIZE                 9
#define ATTR_MESH_CONNECTION_INTERVAL  MSEC_TO_UNITS(100, UNIT_1_25_MS)
#define ATTR_MESH_CONNECTION_TIMEOUT   MSEC_TO_UNITS(500, UNIT_10_MS)
#define ATTR_MESH_SCANNING_INTERVAL    MSEC_TO_UNITS(100, UNIT_0_625_MS)
#define ATTR_MESH_SCANNING_WINDOW      MSEC_TO_UNITS(40, UNIT_0_625_MS)


typedef struct
{
  uint8_t length;
  uint8_t meshName[MESH_NAME_SIZE];
} advertisingData;

extern const ble_gap_conn_params_t meshConnectionParams;
extern const ble_gap_adv_params_t meshAdvertisingParams;
extern const ble_gap_scan_params_t meshScanningParams;

void start_advertising(void);
void stop_advertising(void);

void start_scanning(void);
void stop_scanning(void);

void handle_gap_event(ble_evt_t * bleEvent);
