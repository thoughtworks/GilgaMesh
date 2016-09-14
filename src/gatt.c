#include <gatt.h>
#include <system/ble_gap.h>
#include <message_types/heartbeat_message.h>
#include <message_types/broadcast_message.h>
#include <message_types/handshake_message.h>
#include <ble_conn_state.h>
#include <system/log.h>

typedef struct {
  BleMessageType messageType;
  write_event_handler handler;
} writeEvent;

uint8_t writeEventCount = 0;
ble_gatts_char_handles_t characteristicHandles;
static writeEvent** writeEvents;


void initialize_write_events() {
  add_write_event(Broadcast, receive_broadcast_message);
  add_write_event(Handshake, receive_handshake_message);
  add_write_event(Heartbeat, receive_heartbeat_message);
}

void gatt_initialize() {
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

  initialize_write_events();
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


void send_to_multiple_connections(sdk_mapped_flags_key_list_t handles, uint16_t originHandle, uint8_t *data, uint16_t dataLength) {
  for (int i = 0; i < handles.len; i++){
    uint16_t connectionHandle = handles.flag_keys[i];
    if ((originHandle != connectionHandle) && is_connection_active(connectionHandle)){ //don't resend to the connection who sent it
      connection *targetConnection = find_active_connection_by_handle(connectionHandle);
      send_to_single_connection(targetConnection, data, dataLength);
    }
  }
}

void send_to_central_connection(uint16_t originHandle, uint8_t *data, uint16_t dataLength) {
  send_to_multiple_connections(ble_conn_state_periph_handles(), originHandle, data, dataLength);
}

void send_to_all_connections(uint16_t originHandle, uint8_t *data, uint16_t dataLength) {
  send_to_multiple_connections(ble_conn_state_conn_handles(), originHandle, data, dataLength);
}

void scheduled_broadcast_request(void *data, uint16_t dataLength) {
  send_to_all_connections(BLE_CONN_HANDLE_INVALID, (uint8_t *)data, dataLength);
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
  uint8_t messageType = bleEvent->evt.gatts_evt.params.write.data[0];

  for(int i = 0; i < writeEventCount; i++) {
    writeEvent *registeredEvent = writeEvents[i];
    if(registeredEvent->messageType == messageType) {
      MessagePropagationType propagationType = registeredEvent->handler(bleEvent->evt.gap_evt.conn_handle,
                                                                        bleEvent->evt.gatts_evt.params.write.data);
      if (propagationType == PropagateToCentral) {
        propagate_message_to_central(&bleEvent->evt.gatts_evt);
      }
      else if(propagationType == PropagateToAll) {
        propagate_message(&bleEvent->evt.gatts_evt);
      }
      return;
    }
  }

  MESH_LOG("unknown message type: %d\r\n", messageType);
}

void add_write_event(uint8_t type, write_event_handler handler) {
  writeEvent *newWriteEvent = malloc(sizeof(writeEvent));
  newWriteEvent->messageType = type;
  newWriteEvent->handler = handler;

  writeEventCount++;
  writeEvents = realloc(writeEvents, writeEventCount * sizeof(writeEvent*));
  writeEvents[writeEventCount - 1] = newWriteEvent;
}

