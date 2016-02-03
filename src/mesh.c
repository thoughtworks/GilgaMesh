#include <mesh.h>


void mesh_start_advertising(void)
{
  uint32_t err;

  advertisingData adv_data;
  adv_data.length = strlen(MESH_NAME);
  memcpy(adv_data.meshName, MESH_NAME, adv_data.length);

  err = sd_ble_gap_adv_data_set(&adv_data, sizeof(adv_data), NULL, 0);
  APP_ERROR_CHECK(err);

  log("ADVERTISING: data is now set");


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


void mesh_stop_advertising(void)
{
  uint32_t err = sd_ble_gap_adv_stop();
  APP_ERROR_CHECK(err);

  log("NODE: Advertising stopped");
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


void mesh_stop_scanning(void)
{
  uint32_t err = sd_ble_gap_scan_stop();
  APP_ERROR_CHECK(err);

  log("NODE: Scanning stopped");
}
