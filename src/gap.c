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
  ATTR_MESH_SCANNING_WINDOW,           //window
  0                                    //timeout
};

void sys_evt_dispatch(uint32_t sys_evt)
{
  pstorage_sys_event_handler(sys_evt);
}

void ble_initialize(void){
  uint32_t err = 0;

  log("NODE: Initializing Softdevice");

  EC(softdevice_handler_init(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, currentEventBuffer, sizeOfEvent, NULL));

  EC(softdevice_sys_evt_handler_set(sys_evt_dispatch));

  ble_enable_params_t bleSdEnableParams;
  memset(&bleSdEnableParams, 0, sizeof(bleSdEnableParams));
  bleSdEnableParams.gatts_enable_params.attr_tab_size = ATTR_TABLE_MAX_SIZE;
  bleSdEnableParams.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;

  EC(sd_ble_enable(&bleSdEnableParams));

  EC(sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE));

  EC(sd_power_mode_set(NRF_POWER_MODE_LOWPWR));

  ble_gap_conn_sec_mode_t secPermissionOpen;
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&secPermissionOpen);

  EC(sd_ble_gap_device_name_set(&secPermissionOpen, (uint8_t*)MESH_NAME, strlen(MESH_NAME)));

  EC(sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_COMPUTER));

  EC(sd_ble_gap_ppcp_set(&meshConnectionParams));
}


void start_advertising(void)
{
  uint32_t err;

  advertisingData adv_data;
  adv_data.length = MESH_NAME_SIZE;
  memcpy(adv_data.meshName, MESH_NAME, MESH_NAME_SIZE);

  EC(sd_ble_gap_adv_data_set(&adv_data, sizeof(adv_data), NULL, 0));

  EC(sd_ble_gap_adv_start(&meshAdvertisingParams));

  log("GAP: Advertising started");
}


void stop_advertising(void)
{
  EC(sd_ble_gap_adv_stop());
  log("GAP: Advertising stopped");
}


bool should_connect_to_advertiser(ble_gap_evt_adv_report_t adv_report)
{
  advertisingData* adv_data = (advertisingData *)adv_report.data;

  if (adv_report.dlen != sizeof(advertisingData))return false;
  if (adv_data->length != MESH_NAME_SIZE) return false;
  if (strncmp(adv_data->meshName, MESH_NAME, MESH_NAME_SIZE) != 0) return false;

  if (find_active_connection_by_address(adv_report.peer_addr) != NULL) return false;

  return true;
}


void start_scanning(void)
{
  EC(sd_ble_gap_scan_start(&meshScanningParams));
  log("GAP: Scanning started");
}


void stop_scanning(void)
{
  EC(sd_ble_gap_scan_stop());
  log("GAP: Scanning stopped");
}


void handle_advertising_report_event(ble_gap_evt_adv_report_t advertisingParams)
{
  if (should_connect_to_advertiser(advertisingParams)){
    EC(sd_ble_gap_connect(&advertisingParams.peer_addr, &meshScanningParams, &meshConnectionParams));
  }
}


void handle_connection_event(uint8_t connectionHandle, ble_gap_evt_connected_t connectionParams)
{
  if (find_active_connection_by_address(connectionParams.peer_addr) != NULL){
    // we are already connected to this node, so "undo" this connection event
    EC(sd_ble_gap_disconnect(connectionHandle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
    return;
  }

  if (connectionParams.role == BLE_GAP_ROLE_PERIPH){
    //we are the peripheral, we need to add a central connection
    set_central_connection(connectionHandle, connectionParams.peer_addr);

  } else if (connectionParams.role == BLE_GAP_ROLE_CENTRAL){
    //we are the central, we need to add a peripheral connection
    set_peripheral_connection(connectionHandle, connectionParams.peer_addr);
    start_scanning();

    log("GAP: Writing to our peripheral...");
    write_value(connectionHandle);
  }

  print_all_connections();
}


void handle_disconnection_event(uint8_t connectionHandle)
{
  log("GAP: Received a disconnection event with connection handle %u", connectionHandle);
  ConnectionType lostConnectionType = unset_connection(connectionHandle);
  if (lostConnectionType == CENTRAL){
    start_advertising();
  }
  print_all_connections();
}


void handle_write_event(ble_gatts_evt_write_t writeParams)
{
  log("GATT: Received some data: %d", writeParams.data[0]);
  // now that we've received data from our central, we pass it on to all of our peripherals
}


void handle_gap_event(ble_evt_t * bleEvent)
{
  if (bleEvent->header.evt_id == BLE_GAP_EVT_ADV_REPORT){
    handle_advertising_report_event(bleEvent->evt.gap_evt.params.adv_report);

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_CONNECTED){
    handle_connection_event(bleEvent->evt.gap_evt.conn_handle, bleEvent->evt.gap_evt.params.connected);

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_DISCONNECTED){
    handle_disconnection_event(bleEvent->evt.gap_evt.conn_handle);

  } else if (bleEvent->header.evt_id == BLE_EVT_TX_COMPLETE){
    // no need to handle this, we don't care

  } else if (bleEvent->header.evt_id == BLE_GATTS_EVT_WRITE){
    handle_write_event(bleEvent->evt.gatts_evt.params.write);

  } else {
    log("GAP: Unhandled event: %s", getBleEventNameString(bleEvent->header.evt_id));
  }
}
