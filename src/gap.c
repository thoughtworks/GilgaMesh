#include <gap.h>

const ble_gap_conn_params_t meshConnectionParams =
{
  ATTR_MESH_CONNECTION_INTERVAL,       //min_conn_interval
  ATTR_MESH_CONNECTION_INTERVAL,       //max_conn_interval
  0,                                   //slave_latency
  ATTR_MESH_CONNECTION_TIMEOUT         //conn_sup_timeout
};

const ble_gap_adv_params_t meshAdvertisingParams =
{
  BLE_GAP_ADV_TYPE_ADV_IND,            //type
  0,                                   //p_peer_addr
  BLE_GAP_ADV_FP_ANY,                  //fp
  0,                                   //p_whitelist
  0x0200,                              //interval
  0,                                   //timeout
  {
    0,                                 //channel_mask.ch_37_off
    1,                                 //channel_mask.ch_38_off
    1                                  //channel_mask.ch_39_off
  }
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


void start_advertising(void)
{
  uint32_t err;

  advertisingData adv_data;
  adv_data.length = MESH_NAME_SIZE;
  memcpy(adv_data.meshName, MESH_NAME, MESH_NAME_SIZE);

  err = sd_ble_gap_adv_data_set(&adv_data, sizeof(adv_data), NULL, 0);
  APP_ERROR_CHECK(err);

  err = sd_ble_gap_adv_start(&meshAdvertisingParams);
  APP_ERROR_CHECK(err);

  log("GAP: Advertising started");
}


void stop_advertising(void)
{
  uint32_t err = sd_ble_gap_adv_stop();
  APP_ERROR_CHECK(err);

  log("GAP: Advertising stopped");
}


bool should_connect_to_advertiser(ble_gap_evt_adv_report_t adv_report)
{
  advertisingData* adv_data = (advertisingData *)adv_report.data;

  if (adv_report.dlen != sizeof(advertisingData))return false;
  if (adv_data->length != MESH_NAME_SIZE) return false;
  if (strncmp(adv_data->meshName, MESH_NAME, MESH_NAME_SIZE) != 0) return false;

  return true;
}


void start_scanning(void)
{
  uint32_t err = sd_ble_gap_scan_start(&meshScanningParams);
  APP_ERROR_CHECK(err);

  log("GAP: Scanning started");
}


void stop_scanning(void)
{
  uint32_t err = sd_ble_gap_scan_stop();
  APP_ERROR_CHECK(err);

  log("GAP: Scanning stopped");
}


void handle_gap_event(ble_evt_t * bleEvent)
{
  if (bleEvent->header.evt_id == BLE_GAP_EVT_ADV_REPORT){
    ble_gap_evt_adv_report_t adv_report = bleEvent->evt.gap_evt.params.adv_report;

    if (should_connect_to_advertiser(adv_report)){
      uint32_t err = sd_ble_gap_connect(&adv_report.peer_addr, &meshScanningParams, &meshConnectionParams);
      APP_ERROR_CHECK(err);
    }

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_CONNECTED){
    if (bleEvent->evt.gap_evt.params.connected.role == BLE_GAP_ROLE_PERIPH){
      //we are the peripheral, we need to add a central connection
      set_central_connection(bleEvent->evt.gap_evt.params.connected.peer_addr);

    } else if (bleEvent->evt.gap_evt.params.connected.role == BLE_GAP_ROLE_CENTRAL){
      //we are the central, we need to add a peripheral connection
      set_peripheral_connection(bleEvent->evt.gap_evt.params.connected.peer_addr);
    }

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_DISCONNECTED){
    log("GAP: I've been disconnected! Drat!");
    //later we should clear the stored connection, but for now we'll just turn LEDs off
    led_red_off();
    led_green_off();

  } else {
    log("GAP: I received an unhandled event: %s", getBleEventNameString(bleEvent->header.evt_id));
  }
}
