#include <mesh.h>

void mesh_start_advertising(void)
{
  uint32_t err;
  ble_gap_adv_params_t advertisingParams;

  advertisingParams.type = BLE_GAP_ADV_TYPE_ADV_SCAN_IND;
  advertisingParams.p_peer_addr = 0;
  advertisingParams.fp = BLE_GAP_ADV_FP_ANY;
  advertisingParams.p_whitelist = 0;
  advertisingParams.interval = 0x0200;
  advertisingParams.timeout = 0;
  advertisingParams.channel_mask.ch_37_off = 0;
  advertisingParams.channel_mask.ch_38_off = 1;
  advertisingParams.channel_mask.ch_39_off = 1;

  err = sd_ble_gap_adv_start(&advertisingParams);
  APP_ERROR_CHECK(err);

  log("NODE: Advertising started");
}


void mesh_start_scanning(void)
{
  uint32_t err;
  ble_gap_scan_params_t scanningParams;

  scanningParams.active = 0;
  scanningParams.selective = 0;
  scanningParams.p_whitelist = 0;
  scanningParams.interval = MSEC_TO_UNITS(100, UNIT_0_625_MS);
  scanningParams.window = MSEC_TO_UNITS(40, UNIT_0_625_MS);
  scanningParams.timeout = 0;

  err = sd_ble_gap_scan_start(&scanningParams);
  APP_ERROR_CHECK(err);

  log("NODE: Scanning started");
}
