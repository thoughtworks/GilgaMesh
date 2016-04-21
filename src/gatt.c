#include <gatt.h>
#include <version.h>
#include <stdlib.h>
#include <pn532.h>

ble_gatts_char_handles_t characteristicHandles;

void log_heartbeat_info(BleMessageHeartbeatReq *request);

void gatt_initialize()
{
  ble_uuid_t serviceUUID;
  memset(&serviceUUID, 0, sizeof(serviceUUID));

  ble_uuid128_t baseUUID128 = { FOO_SERVICE_UUID };

  EC(sd_ble_uuid_vs_add(&baseUUID128, &serviceUUID.type));

  uint16_t serviceHandle = 0;

  EC(sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &serviceUUID, &serviceHandle));

  ble_gatts_attr_md_t characteristicConfigurationDescriptor;
  memset(&characteristicConfigurationDescriptor, 0, sizeof(characteristicConfigurationDescriptor));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&characteristicConfigurationDescriptor.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&characteristicConfigurationDescriptor.write_perm);
  characteristicConfigurationDescriptor.vloc = BLE_GATTS_VLOC_STACK;

  ble_gatts_char_md_t characteristicMetadata;
  memset(&characteristicMetadata, 0, sizeof(characteristicMetadata));
  characteristicMetadata.char_props.read = 1;
  characteristicMetadata.char_props.write = 1;
  characteristicMetadata.char_props.write_wo_resp = 1;
  characteristicMetadata.char_props.notify = 0;

  characteristicMetadata.p_char_user_desc = FOO_CHARACTERISTIC_DESCRIPTION;
  characteristicMetadata.char_user_desc_size = sizeof(FOO_CHARACTERISTIC_DESCRIPTION);
  characteristicMetadata.char_user_desc_max_size = sizeof(FOO_CHARACTERISTIC_DESCRIPTION);
  characteristicMetadata.p_cccd_md = &characteristicConfigurationDescriptor;

  ble_uuid_t characteristicUUID;
  characteristicUUID.type = serviceUUID.type;
  characteristicUUID.uuid = FOO_CHARACTERISTIC_UUID;

  ble_gatts_attr_md_t characteristicAttributeMetadata;
  memset(&characteristicAttributeMetadata, 0, sizeof(characteristicAttributeMetadata));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&characteristicAttributeMetadata.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&characteristicAttributeMetadata.write_perm);
  characteristicAttributeMetadata.vlen = 1;
  characteristicAttributeMetadata.vloc = BLE_GATTS_VLOC_STACK;
  characteristicAttributeMetadata.rd_auth = 0;
  characteristicAttributeMetadata.wr_auth = 0;

  ble_gatts_attr_t characteristicAttribute;
  memset(&characteristicAttribute, 0, sizeof(characteristicAttribute));
  characteristicAttribute.p_uuid = &characteristicUUID;
  characteristicAttribute.p_attr_md = &characteristicAttributeMetadata;
  characteristicAttribute.init_len = 0;
  characteristicAttribute.init_offs = 0;
  characteristicAttribute.max_len = FOO_CHARACTERISTIC_MAX_LENGTH;

  memset(&characteristicHandles, 0, sizeof(characteristicHandles));

  EC(sd_ble_gatts_characteristic_add(serviceHandle, &characteristicMetadata, &characteristicAttribute, &characteristicHandles));
}


void send_to_single_connection(uint16_t connectionHandle, uint8_t *data, uint16_t dataLength)
{
  ble_gattc_write_params_t writeParams;
  memset(&writeParams, 0, sizeof(writeParams));
  writeParams.write_op = BLE_GATT_OP_WRITE_CMD;
  writeParams.handle = characteristicHandles.value_handle;
  writeParams.offset = 0;
  writeParams.len = dataLength;
  writeParams.p_value = data;

  EC(sd_ble_gattc_write(connectionHandle, &writeParams));
}


void send_to_all_connections(uint16_t originHandle, uint8_t *data, uint16_t dataLength)
{
  uint8_t connectionCount;
  uint16_t connectionHandles[ATTR_MAX_CONNECTIONS];
  get_active_connection_handles(connectionHandles, &connectionCount);
  for (int i = 0; i < connectionCount; i++){
    if (originHandle != connectionHandles[i]){ //don't resend to the connection who sent it
      send_to_single_connection(connectionHandles[i], data, dataLength);
    }
  }
}


void propagate_family_id(uint16_t originHandle)
{
  BleMessageSetFamilyIDReq request;
  memset(&request, 0, sizeof(request));
  request.familyID = familyId;
  request.head.messageType = SetFamilyID;
  uint8_t *data = (uint8_t *) &request;

  send_to_all_connections(originHandle, data, sizeof(request));
}


void broadcast_message(char* message)
{
  if (strlen(message) > BROADCAST_SIZE) {
    log("ERROR: Broadcast message longer than %u characters", BROADCAST_SIZE);
    return;
  }

  BleMessageBroadcastReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Broadcast;
  memcpy(request.message, message, BROADCAST_SIZE);

  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *)&request, sizeof(request));
}


void log_heartbeat_info(BleMessageHeartbeatReq *request)
{
  char *nodeName = malloc(NODE_NAME_SIZE);
  char* parentNodeName;

  set_node_name_from_device_id(request->deviceId, nodeName);
  if (request->centralConnectionDeviceId == 0) {
    parentNodeName = "ROOT";
  } else {
    parentNodeName = malloc(NODE_NAME_SIZE);
    set_node_name_from_device_id(request->centralConnectionDeviceId, parentNodeName);
  }

  log("HEARTBEAT: {\"id\": \"%s\", \"parentId\": \"%s\", \"family\": %u, \"version\": %u.%u, \"votes\": %u}",
      nodeName, parentNodeName, familyId, request->majorVersion, request->minorVersion, request->voteCount);

}


void broadcast_heartbeat()
{
  BleMessageHeartbeatReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Heartbeat;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = deviceId;
  if (central_connection_active() && (activeConnections->central.deviceId != 0)) {
    request.centralConnectionDeviceId = activeConnections->central.deviceId;
  }
  request.voteCount = voteCount;

  log_and_propagate_heartbeat(BLE_CONN_HANDLE_INVALID, &request);
}


void log_and_propagate_heartbeat(uint16_t originHandle, BleMessageHeartbeatReq *heartbeat) {
  log_heartbeat_info(heartbeat);
  send_to_all_connections(originHandle, (uint8_t *)heartbeat, sizeof(BleMessageHeartbeatReq));
}


void broadcast_vote(unsigned short voterId)
{
  BleMessageVoteReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Vote;
  request.deviceId = deviceId;
  request.voterId = voterId;

  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *)&request, sizeof(BleMessageVoteReq));
}


void log_and_propagate_vote(uint16_t originHandle, BleMessageVoteReq *request) {
  char *nodeName = malloc(NODE_NAME_SIZE);
  set_node_name_from_device_id(request->deviceId, nodeName);

  log("VOTE: {\"node\": \"%s\", \"voterId\": \"%u\"}", nodeName, request->voterId);
  send_to_all_connections(originHandle, (uint8_t *)request, sizeof(BleMessageVoteReq));
}


void share_connection_info(uint16_t targetHandle)
{
  BleMessageConnectionInfoReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = ConnectionInfo;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = deviceId;

  send_to_single_connection(targetHandle, (uint8_t *)&request, sizeof(request));
}
