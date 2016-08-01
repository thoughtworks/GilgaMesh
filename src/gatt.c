#include <gatt.h>
#include <gap.h>
#include <timer.h>
#include <heartbeat.h>

ble_gatts_char_handles_t characteristicHandles;

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

//  turn_on_heartbeats();
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


void send_to_single_connection(connection *targetConnection, uint8_t *data, uint16_t dataLength)
{
  if (push_onto_queue(&targetConnection->unsentMessages, data, dataLength)) {
    send_message_from_connection_queue(targetConnection);
  } else {
    disconnect_from_peer(targetConnection->handle);
  }
}


void send_to_central_connection(uint16_t originHandle, uint8_t *data, uint16_t dataLength) {
  connection *centralConnection = &activeConnections->central;
  if (centralConnection->active && (originHandle != centralConnection->handle)){ //don't resend to the connection who sent it
    send_to_single_connection(centralConnection, data, dataLength);
  }
}


void send_to_all_connections(uint16_t originHandle, uint8_t *data, uint16_t dataLength)
{
  send_to_central_connection(originHandle, data, dataLength);

  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    connection *targetConnection = &activeConnections->peripheral[i];
    if (targetConnection->active && (originHandle != targetConnection->handle)){ //don't resend to the connection who sent it
      send_to_single_connection(targetConnection, data, dataLength);
    }
  }
}


void scheduled_broadcast_request(void *data, uint16_t dataLength) {
  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *)data, dataLength);
}


void broadcast_logging_message(char **parsedCommandArray)
{
  if (strlen(parsedCommandArray[0]) > BROADCAST_SIZE) {
    NRF_LOG_PRINTF("ERROR: Broadcast message longer than %u characters\r\n", BROADCAST_SIZE);
    return;
  }

  BleMessageBroadcastReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Broadcast;
  memcpy(request.message, parsedCommandArray[0], BROADCAST_SIZE);

  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(BleMessageBroadcastReq));
}


void propagate_message(ble_gatts_evt_t *gattsEvent) {
  uint16_t originHandle = gattsEvent->conn_handle;
  uint8_t *request = gattsEvent->params.write.data;
  uint16_t dataLength = gattsEvent->params.write.len;
  send_to_all_connections(originHandle, request, dataLength);
}


void propagate_message_to_central(ble_gatts_evt_t *gattsEvent) {
  uint16_t originHandle = gattsEvent->conn_handle;
  uint8_t *request = gattsEvent->params.write.data;
  uint16_t dataLength = gattsEvent->params.write.len;
  send_to_central_connection(originHandle, request, dataLength);
}


void handle_write_event(void * data, uint16_t dataLength)
{
  UNUSED_PARAMETER(dataLength);
  ble_evt_t *bleEvent = (ble_evt_t *)data;

  BleMessageHead* head = (BleMessageHead*) bleEvent->evt.gatts_evt.params.write.data;

  switch (head->messageType) {
    case StartDFU:  // reboot the device into dfu mode
//      restart_in_bootloader_mode();
      break;
    case SetFamilyID: // no longer used
      break;
    case Broadcast:
      receive_broadcast_message(bleEvent->evt.gatts_evt.params.write.data);
      propagate_message(&bleEvent->evt.gatts_evt);
      break;
    case ConnectionInfo:
      update_connection_info(bleEvent->evt.gap_evt.conn_handle, bleEvent->evt.gatts_evt.params.write.data);
      break;
    case Heartbeat:
//      receive_heartbeat(bleEvent->evt.gatts_evt.params.write.data);
      propagate_message_to_central(&bleEvent->evt.gatts_evt);
      break;
//    case Vote:
//      receive_vote(bleEvent->evt.gatts_evt.params.write.data);
//      propagate_message_to_central(&bleEvent->evt.gatts_evt);
//      break;
//    case VoteAcknowledgement:
//      if(!receive_vote_acknowledgement(bleEvent->evt.gatts_evt.params.write.data)) {
//        propagate_message(&bleEvent->evt.gatts_evt);
//      }
//      break;
//    case SetVoteConfiguration:
//      if(!receive_group_value_update(bleEvent->evt.gatts_evt.params.write.data)) {
//        propagate_message(&bleEvent->evt.gatts_evt);
//
//      break;
    default:
      NRF_LOG_PRINTF("unknown message type: %d\r\n", head->messageType);
      break;
  }
}

