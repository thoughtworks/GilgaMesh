#include "gap.h"
#include <app_scheduler.h>
#include "message_types/handshake_message.h"


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


const ble_gap_scan_params_t meshConnectionScanningParams =
{
  0,                                   //active
  0,                                   //selective
  0,                                   //p_whitelist
  ATTR_MESH_SCANNING_INTERVAL,         //interval
  ATTR_MESH_SCANNING_WINDOW,           //window
  ATTR_MESH_CONNECTION_SCAN_TIMEOUT    //timeout
};


void gap_initialize(void){

  EC(sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE));

  EC(sd_power_mode_set(NRF_POWER_MODE_LOWPWR));

  ble_gap_conn_sec_mode_t secPermissionOpen;
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&secPermissionOpen);

  EC(sd_ble_gap_device_name_set(&secPermissionOpen, (const uint8_t *)nodeName, NODE_NAME_SIZE));

  EC(sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_COMPUTER));

  EC(sd_ble_gap_ppcp_set(&meshConnectionParams));
}


void set_advertising_data() {
  advertisingManufacturerData manufacturerData;
  manufacturerData.length = sizeof(manufacturerData) - 1;
  manufacturerData.typeDefinition = BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
  manufacturerData.manufacturerId = MANUFACTURER_ID;
  manufacturerData.majorVersion = APP_VERSION_MAIN;
  manufacturerData.minorVersion = APP_VERSION_SUB;

  advertisingDeviceData deviceData;
  deviceData.length = sizeof(deviceData) - 1;
  deviceData.typeDefinition = BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME;
  memcpy(deviceData.deviceName, nodeName, NODE_NAME_SIZE);

  advertisingServiceData serviceData;
  serviceData.length = sizeof(serviceData) - 1;
  serviceData.typeDefinition = BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE;
  uint8_t serviceUUID[16] = {FOO_SERVICE_UUID};
  memcpy(serviceData.serviceUUID, serviceUUID, 16);

  advertisingData advData;
  advData.manufacturerData = manufacturerData;
  advData.deviceData = deviceData;

  scanResponseData scanData;
  scanData.serviceData = serviceData;

  EC(sd_ble_gap_adv_data_set((const uint8_t *)&advData, sizeof(advData), (const uint8_t *)&scanData, sizeof(scanData)));
}


void start_advertising(void) {
#ifdef IS_PROD_BOARD
  set_advertising_data();
  EC(sd_ble_gap_adv_start(&meshAdvertisingParams));
#endif
}


void check_advertising_status(char** parserCommandArray) {
  if (central_connection_active()) {
    NRF_LOG_PRINTF("Advertising status: NOT APPLICABLE - central connection slot filled\r\n");
  } else {
    uint32_t errorCode = sd_ble_gap_adv_start(&meshAdvertisingParams);
    if (errorCode == NRF_SUCCESS) {
      NRF_LOG_PRINTF("Advertising status: RESTARTED\r\n");
    } else {
      NRF_LOG_PRINTF("Advertising status: RUNNING OR UNABLE TO START\r\n");
    }
  }
}


void check_scanning_status(char** parserCommandArray) {
  if (all_peripheral_connections_active()) {
    NRF_LOG_PRINTF("Scanning status NOT APPLICABLE: peripheral connection slots filled\r\n");
  } else {
    uint32_t errorCode = sd_ble_gap_scan_start(&meshScanningParams);
    if (errorCode == NRF_SUCCESS) {
      NRF_LOG_PRINTF("Scanning status: RESTARTED\r\n");
    } else {
      NRF_LOG_PRINTF("Scanning status: RUNNING OR UNABLE TO START\r\n");
    }
  }
}


bool should_connect_to_advertiser(ble_gap_evt_adv_report_t *adv_report) {
  advertisingData* adv_data = (advertisingData *)adv_report->data;

  if (adv_report->dlen != sizeof(advertisingData)) return false;
  if (adv_data->manufacturerData.typeDefinition != BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA) return false;
  if (adv_data->manufacturerData.manufacturerId != MANUFACTURER_ID) return false;

  NRF_LOG_PRINTF("UNCONNECTED NODE: {\"id\": \"%s\", \"version\": %u.%u}\r\n",
      adv_data->deviceData.deviceName, adv_data->manufacturerData.majorVersion,
      adv_data->manufacturerData.minorVersion);

  return true;
}


void start_scanning(void) {
  EC(sd_ble_gap_scan_start(&meshScanningParams));
}


void stop_scanning(void) {
  EC(sd_ble_gap_scan_stop());
}


void connect_to_peer(ble_gap_addr_t *peerAddress) {
  stop_scanning();
  uint32_t errorCode = sd_ble_gap_connect(peerAddress, &meshConnectionScanningParams, &meshConnectionParams);
  if (errorCode != NRF_SUCCESS) {
    start_scanning();
  }
  EC(errorCode);
}


void handle_advertising_report_event(void * bleEvent, uint16_t dataLength) {
  UNUSED_PARAMETER(dataLength);
  ble_gap_evt_adv_report_t advertisingParams = ((ble_evt_t *)bleEvent)->evt.gap_evt.params.adv_report;
  if (should_connect_to_advertiser(&advertisingParams) && !all_peripheral_connections_active()) {
    connect_to_peer(&advertisingParams.peer_addr);
  }
}


void disconnect_from_peer(uint16_t connectionHandle) {
  EC(sd_ble_gap_disconnect(connectionHandle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
}


void disconnect_from_all_peers() {
  if (central_connection_active()) disconnect_from_peer(activeConnections->central.handle);
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    connection *peripheralConnection = &activeConnections->peripheral[i];
    if (peripheralConnection->active) disconnect_from_peer(peripheralConnection->handle);
  }
}


void handle_connection_event(void * data, uint16_t dataLength) {
  UNUSED_PARAMETER(dataLength);
  ble_evt_t *bleEvent = (ble_evt_t *)data;
  ble_gap_evt_connected_t connectionParams = bleEvent->evt.gap_evt.params.connected;

  connection *newConnection;
  if (connectionParams.role == BLE_GAP_ROLE_PERIPH){
    //we are the peripheral, we need to add a central connection
    newConnection = set_central_connection(bleEvent->evt.gap_evt.conn_handle, connectionParams.peer_addr);

  } else {
    //we are the central, we need to add a peripheral connection
    newConnection = set_peripheral_connection(bleEvent->evt.gap_evt.conn_handle, connectionParams.peer_addr);
#ifdef IS_PROD_BOARD
    if (!central_connection_active()) {
      disconnect_from_peer(bleEvent->evt.gap_evt.conn_handle);
      return;
    }
#endif
  }

  send_handshake_message(newConnection);
  print_all_connections(NULL);

#ifdef IS_PROD_BOARD
  if (!all_peripheral_connections_active()) start_scanning();
#else
  start_scanning();
#endif
}


void handle_disconnection_event(void * bleEvent, uint16_t dataLength) {
  UNUSED_PARAMETER(dataLength);
  ConnectionType lostConnectionType = unset_connection(((ble_evt_t *)bleEvent)->evt.gap_evt.conn_handle);

  if (lostConnectionType == CENTRAL) { // we lost our parent, so the mesh is no more
    disconnect_from_all_peers();
    start_advertising();
    stop_scanning();
  } else if (lostConnectionType == PERIPHERAL) { //we lost our child, scan for more
    start_scanning();
  }

  print_all_connections(NULL);
}


void handle_connection_timeout_event(void * data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  start_scanning();
}


void handle_buffers_freed_event(void * data, uint16_t dataLength)
{
  UNUSED_PARAMETER(dataLength);
  ble_evt_t *bleEvent = (ble_evt_t *)data;

  uint8_t freeBuffers = bleEvent->evt.common_evt.params.tx_complete.count;
  connection *targetConnection = find_active_connection_by_handle(bleEvent->evt.common_evt.conn_handle);
  for (int i = 0; i < freeBuffers; i++) {
    send_message_from_connection_queue(targetConnection);
  }
}


void handle_gap_event(ble_evt_t * bleEvent)
{
  if (bleEvent->header.evt_id == BLE_GAP_EVT_ADV_REPORT){
    EC(app_sched_event_put(bleEvent, bleEvent->header.evt_len, handle_advertising_report_event));

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_CONNECTED){
    EC(app_sched_event_put(bleEvent, bleEvent->header.evt_len, handle_connection_event));

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_DISCONNECTED){
    EC(app_sched_event_put(bleEvent, bleEvent->header.evt_len, handle_disconnection_event));

  } else if (bleEvent->header.evt_id == BLE_GATTS_EVT_WRITE){
    EC(app_sched_event_put(bleEvent, bleEvent->header.evt_len, handle_write_event));

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_TIMEOUT){
    EC(app_sched_event_put(bleEvent, bleEvent->header.evt_len, handle_connection_timeout_event));

  } else if (bleEvent->header.evt_id == BLE_EVT_TX_COMPLETE){
    EC(app_sched_event_put(bleEvent, bleEvent->header.evt_len, handle_buffers_freed_event));

  } else {
    NRF_LOG_PRINTF("GAP: Unhandled event: %s\r\n", getBleEventNameString(bleEvent->header.evt_id));
  }
}
