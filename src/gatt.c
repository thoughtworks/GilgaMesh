#include <gatt.h>
#include <version.h>
#include <votes.h>
#include <sdk_common.h>
#include <gap.h>

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


void send_message_from_connection_queue(connection *targetConnection)
{
  if (targetConnection == NULL) return;
  if (queue_is_empty(&targetConnection->unsentMessages)) return;

  uint8_t *p_data = malloc(MAX_QUEUE_DATA_LENGTH);
  uint16_t dataLength = 0;

  if (peek_from_queue(&targetConnection->unsentMessages, p_data, &dataLength)) {
    ble_gattc_write_params_t writeParams;
    memset(&writeParams, 0, sizeof(writeParams));
    writeParams.write_op = BLE_GATT_OP_WRITE_CMD;
    writeParams.handle = characteristicHandles.value_handle;
    writeParams.offset = 0;
    writeParams.len = dataLength;
    writeParams.p_value = p_data;

    uint32_t errorCode;
    errorCode = sd_ble_gattc_write(targetConnection->handle, &writeParams);
    if (errorCode == NRF_SUCCESS) {
      pop_from_queue(&targetConnection->unsentMessages);
    }
  }
  free(p_data);
}


void send_to_single_connection(connection *targetConnection, uint8_t *data, uint16_t dataLength, messagePriority priority)
{
  if (push_onto_queue(&targetConnection->unsentMessages, data, dataLength, priority)) {
    send_message_from_connection_queue(targetConnection);
  } else {
    disconnect_from_peer(targetConnection->handle);
  }
}


void send_to_all_connections(uint16_t originHandle, uint8_t *data, uint16_t dataLength, messagePriority priority)
{
  connection *targetConnection = &activeConnections->central;
  if (targetConnection->active && (originHandle != targetConnection->handle)){ //don't resend to the connection who sent it
    send_to_single_connection(targetConnection, data, dataLength, priority);
  }

  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    targetConnection = &activeConnections->peripheral[i];
    if (targetConnection->active && (originHandle != targetConnection->handle)){ //don't resend to the connection who sent it
      send_to_single_connection(targetConnection, data, dataLength, priority);
    }
  }
}


void propagate_family_id(uint16_t originHandle)
{
  BleMessageSetFamilyIDReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = SetFamilyID;
  request.familyID = familyId;

  send_to_all_connections(originHandle, (uint8_t *) &request, sizeof(request), MESSAGE_PRIORITY_HIGH);
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

  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *)&request, sizeof(request), MESSAGE_PRIORITY_NORMAL);
}


void log_heartbeat_info(BleMessageHeartbeatReq *request)
{
  char *nodeName = malloc(NODE_NAME_SIZE);
  char* parentNodeName = malloc(NODE_NAME_SIZE);

  set_node_name_from_device_id(request->deviceId, nodeName);
  if (request->centralConnectionDeviceId == 0) {
    strncpy(parentNodeName, "ROOT", 5);
  } else {
    set_node_name_from_device_id(request->centralConnectionDeviceId, parentNodeName);
  }

  char *nfcEnabled = (request->nfc == NFC_STATUS_DISABLED) ? "false" : "true";
  char *nfcWorking = (request->nfc != NFC_STATUS_WORKING)  ? "false" : "true";

  log("HEARTBEAT: {\"id\": \"%s\", \"parentId\": \"%s\", \"family\": %u, \"version\": %u.%u, \"votes\": %u, "
              "\"nfcEnabled\": %s, \"nfcWorking\": %s}",
      nodeName, parentNodeName, familyId, request->majorVersion, request->minorVersion, request->voteCount,
      nfcEnabled, nfcWorking);

  free(nodeName);
  free(parentNodeName);
}


void broadcast_heartbeat(void *data, uint16_t dataLength)
{
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  BleMessageHeartbeatReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Heartbeat;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = deviceId;
  if (central_connection_active() && (activeConnections->central.deviceId != 0)) {
    request.centralConnectionDeviceId = activeConnections->central.deviceId;
  }
  request.voteCount = get_vote_count();
  request.nfc = get_nfc_status();

  log_and_propagate_heartbeat(BLE_CONN_HANDLE_INVALID, &request);
}


void log_and_propagate_heartbeat(uint16_t originHandle, BleMessageHeartbeatReq *heartbeat) {
  log_heartbeat_info(heartbeat);
  send_to_all_connections(originHandle, (uint8_t *)heartbeat, sizeof(BleMessageHeartbeatReq), MESSAGE_PRIORITY_NORMAL);
}


void broadcast_vote(unsigned short voterId)
{
  BleMessageVoteReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Vote;
  request.deviceId = deviceId;
  request.voterId = voterId;

  log_and_propagate_vote(BLE_CONN_HANDLE_INVALID, &request);
}


void log_and_propagate_vote(uint16_t originHandle, BleMessageVoteReq *request) {
  char *votingNodeName = malloc(NODE_NAME_SIZE);
  set_node_name_from_device_id(request->deviceId, votingNodeName);

  log("VOTE: {\"node\": \"%s\", \"voterId\": \"%u\"}", votingNodeName, request->voterId);
  free(votingNodeName);
  send_to_all_connections(originHandle, (uint8_t *)request, sizeof(BleMessageVoteReq), MESSAGE_PRIORITY_NORMAL);
}


void share_connection_info(connection *targetConnection)
{
  BleMessageConnectionInfoReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = ConnectionInfo;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = deviceId;

  send_to_single_connection(targetConnection, (uint8_t *)&request, sizeof(request), MESSAGE_PRIORITY_NORMAL);
}
