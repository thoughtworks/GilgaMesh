#include <mesh.h>

const ble_gap_conn_params_t meshConnectionParams =
{
  ATTR_MESH_CONNECTION_INTERVAL,       //min_conn_interval
  ATTR_MESH_CONNECTION_INTERVAL,       //max_conn_interval
  0,                                   //slave_latency
  ATTR_MESH_CONNECTION_TIMEOUT         //conn_sup_timeout
};

const ble_gap_scan_params_t meshScanningParams =
{
  0,                                   //active
  0,                                   //selective
  0,                                   //p_whitelist
  ATTR_MESH_SCANNING_INTERVAL,         //interval
  ATTR_MESH_SCANNING_INTERVAL,         //window
  0                                    //timeout
};


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


bool should_connect_to_advertiser(ble_gap_evt_adv_report_t adv_report)
{
  advertisingData* adv_data = (advertisingData *)adv_report.data;

  if (adv_report.dlen != sizeof(advertisingData))return false;
  if (adv_data->length != MESH_NAME_SIZE) return false;
  if (strncmp(adv_data->meshName, MESH_NAME, MESH_NAME_SIZE) != 0) return false;

  // Flash blue for now so that we know we found a possible connection
  led_blue_on();
  nrf_delay_us(50000);
  led_blue_off();

  log("ADV_REPORT_EVENT: I found a friend for %s!", MESH_NAME);
  return true;
}


void mesh_start_scanning(void)
{
  uint32_t err = sd_ble_gap_scan_start(&meshScanningParams);
  APP_ERROR_CHECK(err);

  log("NODE: Scanning started");
}


void mesh_stop_scanning(void)
{
  uint32_t err = sd_ble_gap_scan_stop();
  APP_ERROR_CHECK(err);

  log("NODE: Scanning stopped");
}
