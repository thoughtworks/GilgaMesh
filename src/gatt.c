#include <gatt.h>


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
}


void write_value(uint16_t connectionHandle, uint8_t *data, uint16_t dataLength)
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


void propagate_family_id(uint16_t originHandle)
{
  uint8_t connectionCount;
  uint16_t connectionHandles[4];
  get_active_connection_handles(connectionHandles, &connectionCount);
  for (int i = 0; i < connectionCount; i++){
    if (originHandle != connectionHandles[i]){ //don't resend to the node who sent it
      log("Sending familyId %u to connection handle %u", familyId, connectionHandles[i]);

      BleMessageSetFamilyIDReq request;
      memset(&request, 0, sizeof(request));
      request.familyID = familyId;
      request.head.messageType = SetFamilyID;
      write_value(connectionHandles[i], (uint8_t *)&request, sizeof(request));
    }
  }
}
