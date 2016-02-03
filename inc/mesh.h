#include <string.h>

#include <nrf_delay.h>
#include <app_util.h>
#include <app_error.h>
#include <ble.h>
#include <ble_gap.h>

#include <logger.h>

#define MESH_NAME             "MeshyMesh"
#define MESH_NAME_SIZE        9


typedef struct
{
  uint8_t length;
  uint8_t meshName[MESH_NAME_SIZE];
} advertisingData;


void mesh_start_advertising(void);
void mesh_stop_advertising(void);
bool should_connect_to_advertiser(ble_gap_evt_adv_report_t adv_report);

void mesh_start_scanning(void);
void mesh_stop_scanning(void);
