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
  log("Initializing Softdevice");

  EC(softdevice_handler_init(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, currentEventBuffer, sizeOfEvent, 0));

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

  EC(sd_ble_gap_device_name_set(&secPermissionOpen, nodeName, NODE_NAME_SIZE));

  EC(sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_COMPUTER));

  EC(sd_ble_gap_ppcp_set(&meshConnectionParams));
}


void start_advertising(void)
{
  advertisingManufacturerData manufacturerData;
  manufacturerData.length = sizeof(manufacturerData) - 1;
  manufacturerData.typeDefinition = BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
  manufacturerData.manufacturerId = MANUFACTURER_ID;
  manufacturerData.familyId = familyId;
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

  EC(sd_ble_gap_adv_start(&meshAdvertisingParams));
  log("Advertising started");
}


void stop_advertising(void)
{
  EC(sd_ble_gap_adv_stop());
  log("Advertising stopped");
}


bool should_connect_to_advertiser(ble_gap_evt_adv_report_t *adv_report)
{
  advertisingData* adv_data = (advertisingData *)adv_report->data;

  if (adv_report->dlen != sizeof(advertisingData)) return false;
  if (adv_data->manufacturerData.typeDefinition != BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA) return false;
  if (adv_data->manufacturerData.manufacturerId != MANUFACTURER_ID) return false;
  if (adv_data->manufacturerData.familyId == familyId) return false;

  log("Connecting to node with familyId %u, device name %s", adv_data->manufacturerData.familyId, adv_data->deviceData.deviceName);
  return true;
}


void start_scanning(void)
{
  EC(sd_ble_gap_scan_start(&meshScanningParams));
  log("Scanning started");
}


void stop_scanning(void)
{
  EC(sd_ble_gap_scan_stop());
  log("Scanning stopped");
}


void update_family_id(uint32_t newFamilyId)
{
  if (!central_connection_active()) stop_advertising();

  familyId = newFamilyId;
  log("Updated family id to %u", familyId);

  if (!central_connection_active()) start_advertising();
}


void update_and_propagate_family_id(uint32_t newFamilyId, uint16_t originHandle)
{
  update_family_id(newFamilyId);
  propagate_family_id(originHandle);
}


void handle_advertising_report_event(ble_evt_t * bleEvent)
{
  ble_gap_evt_adv_report_t advertisingParams = bleEvent->evt.gap_evt.params.adv_report;
  if (should_connect_to_advertiser(&advertisingParams)){
    EC(sd_ble_gap_connect(&advertisingParams.peer_addr, &meshScanningParams, &meshConnectionParams));
  }
}


void handle_connection_event(ble_evt_t * bleEvent)
{
  ble_gap_evt_connected_t connectionParams = bleEvent->evt.gap_evt.params.connected;
  if (find_active_connection_by_address(connectionParams.peer_addr) != 0){
    // we are already connected to this node, so "undo" this connection event
    EC(sd_ble_gap_disconnect(bleEvent->evt.gap_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
    return;
  }

  if (connectionParams.role == BLE_GAP_ROLE_PERIPH){
    //we are the peripheral, we need to add a central connection
    set_central_connection(bleEvent->evt.gap_evt.conn_handle, connectionParams.peer_addr);

  } else if (connectionParams.role == BLE_GAP_ROLE_CENTRAL){
    //we are the central, we need to add a peripheral connection
    set_peripheral_connection(bleEvent->evt.gap_evt.conn_handle, connectionParams.peer_addr);
    update_and_propagate_family_id(familyId + 1, BLE_CONN_HANDLE_INVALID);
    start_scanning();
  }

  print_all_connections();
}


void handle_disconnection_event(ble_evt_t * bleEvent)
{
  ConnectionType lostConnectionType = unset_connection(bleEvent->evt.gap_evt.conn_handle);
  if (lostConnectionType == INVALID) return;

  if (lostConnectionType == CENTRAL) start_advertising();
  uint32_t newFamilyId = (lostConnectionType == CENTRAL) ? generate_family_id() : (familyId + 1);
  update_and_propagate_family_id(newFamilyId, BLE_CONN_HANDLE_INVALID);

  print_all_connections();
}

void setFamilyID(ble_evt_t * bleEvent) {
	uint16_t connectionHandle = bleEvent->evt.gap_evt.conn_handle;
	BleMessageSetFamilyIDReq *req = (BleMessageSetFamilyIDReq *) bleEvent->evt.gatts_evt.params.write.data;
	log("***** RECEIVED New family id: %u", req->familyID);

	if (req->familyID == familyId){
	  //we are already connected to this family! we should disconnect
	  log("we received our own family Id from a current connection! Disconnecting...");
	  EC(sd_ble_gap_disconnect(connectionHandle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
	  return;
	}

  update_and_propagate_family_id(req->familyID, connectionHandle);
}


void restartForDFU()
{
  log("StartDFU");
  BleMessageStartDFURsp result;
  result.result.errorCode = 0;
  // Now the device will reboot to bootloader
  sd_power_gpregret_clr(0xFF);
  sd_power_gpregret_set(0xB1);//BOOTLOADER_DFU_START);
  NVIC_SystemReset();
}


ble_gatts_char_handles_t characteristicHandles;

void handle_write_event(ble_evt_t * bleEvent)
{
  uint16_t connectionHandle = bleEvent->evt.gatts_evt.conn_handle;

  BleMessageHead* head = (BleMessageHead*) bleEvent->evt.gatts_evt.params.write.data;

  char ps[7];
  memset(ps, 0, 7);
  memcpy(ps, head->password, 6);
  log("connect handle:%d;password:%s；op:%d;charUUID:%d", connectionHandle, ps,
		  bleEvent->evt.gatts_evt.params.write.op, characteristicHandles.value_handle);

  // check password
  //  if (head->password) {
  //  	  BleMessageResult result;
  //  	  result.errorCode = AN ERROR;
  //  	  result.sessionID = head->sessionID;
  //  	  write_rsp(connectionHandle, &result, sizeof(result));
  //  	  return;
  //  }

  switch (head->messageType) {
    case QueryVersion: { // should return the version of current application.now it is test code
  	  static uint8_t verErrorCode = 1;
  	  BleMessageQueryVersionRsp result;
  	  result.result.errorCode = 0;
  	  result.result.sessionID = head->sessionID;
  	  verErrorCode++;
  	  result.versionMain = APP_VERSION_MAIN;
  	  result.versionSub = APP_VERSION_SUB;
  	  log("QueryVersion with result:%d,%d;session:%d", result.result.errorCode, result.versionMain, head->sessionID);

      write_rsp(connectionHandle, (uint8_t *)&result, sizeof(result));
    }
  	  break;
    case StartDFU:  // reboot the device into dfu mode
      restartForDFU();
  	  break;
    case SetFamilyID:
    	setFamilyID(bleEvent);
    	break;
    default:
      log("unknown message type: %d", head->messageType);
  	  break;
    }
}


void handle_gap_event(ble_evt_t * bleEvent)
{
  if (bleEvent->header.evt_id == BLE_GAP_EVT_ADV_REPORT){
    handle_advertising_report_event(bleEvent);

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_CONNECTED){
    handle_connection_event(bleEvent);

  } else if (bleEvent->header.evt_id == BLE_GAP_EVT_DISCONNECTED){
    handle_disconnection_event(bleEvent);

  } else if (bleEvent->header.evt_id == BLE_GATTS_EVT_WRITE){
    handle_write_event(bleEvent);

  } else if (bleEvent->header.evt_id == BLE_EVT_TX_COMPLETE){
    //no need to do anything, just swallow the event

  } else {
    log("GAP: Unhandled event: %s", getBleEventNameString(bleEvent->header.evt_id));
  }
}
