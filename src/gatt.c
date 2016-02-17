#include <gatt.h>

#define MESH_SERVICE_BASE_UUID128 0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
#define MESH_SERVICE_UUID 0x1523
#define MESH_SERVICE_CHARACTERISTIC_UUID 0x1524
#define MESH_CHARACTERISTIC_MAX_LENGTH 10
#define SERVICE_CHARACTERISTIC_DESCRIPTOR_UUID  0x9002
#define SERVICE_CHARACTERISTIC_DESC_VALUE       "Average"

uint8_t WATEVER[] = {0,1,2,3,4};

ble_gatts_char_handles_t characteristicHandles;

void gatt_initialize()
{
  /*
  ble_uuid_t serviceUUID;
  memset(&serviceUUID, 0, sizeof(serviceUUID));

  ble_uuid128_t baseUUID128 = { FOO_SERVICE_UUID };

  log("GATT: Adding our UUID");
  EC(sd_ble_uuid_vs_add(&baseUUID128, &serviceUUID.type));

  uint16_t serviceHandle = 0;

  log("GATT: Adding a service");
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

  log("size of characteristicHandles: %d", sizeof(characteristicHandles));
  memset(&characteristicHandles, 0, sizeof(characteristicHandles));

  log("GATT: Adding a characteristic");
  EC(sd_ble_gatts_characteristic_add(serviceHandle, &characteristicMetadata, &characteristicAttribute, &characteristicHandles));

  log("Our characteristic handle is: %u", characteristicHandles.value_handle);

  */


	uint32_t err = 0;

	meshService.connectionHandle = BLE_CONN_HANDLE_INVALID;

	//##### At first, we register our custom service
	//Add our Service UUID to the BLE stack for management
	ble_uuid128_t baseUUID128 = { MESH_SERVICE_BASE_UUID128 };
	err = sd_ble_uuid_vs_add(&baseUUID128, &meshService.serviceUuid.type);
	APP_ERROR_CHECK(err);

	//Add the service
	err = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &meshService.serviceUuid, &meshService.serviceHandle);
	APP_ERROR_CHECK(err);

	//##### Now we need to add a characteristic to that service

	//BLE GATT Attribute Metadata http://developer.nordicsemi.com/nRF51_SDK/doc/7.1.0/s120/html/a00163.html
	//Read and write permissions, variable length, etc...
	ble_gatts_attr_md_t attributeMetadata;
	memset(&attributeMetadata, 0, sizeof(ble_gatts_attr_md_t));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attributeMetadata.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attributeMetadata.write_perm);

	attributeMetadata.vloc = BLE_GATTS_VLOC_STACK; //We currently have the value on the SoftDevice stack, we might port that to the application space
	attributeMetadata.rd_auth = 0;
	attributeMetadata.wr_auth = 0;
	attributeMetadata.vlen = 0;

	//Client Characteristic Configuration Descriptor, whatever....
	ble_gatts_attr_md_t clientCharacteristicConfigurationDescriptor;
	memset(&clientCharacteristicConfigurationDescriptor, 0, sizeof(ble_gatts_attr_md_t));
	clientCharacteristicConfigurationDescriptor.vloc = BLE_GATTS_VLOC_STACK;
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&clientCharacteristicConfigurationDescriptor.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&clientCharacteristicConfigurationDescriptor.write_perm);

  ble_gatts_char_pf_t clientCharacteristicPresentationFormat;
  memset(&clientCharacteristicPresentationFormat, 0, sizeof(clientCharacteristicPresentationFormat));
  clientCharacteristicPresentationFormat.format = BLE_GATT_CPF_FORMAT_STRUCT;

	//Characteristic metadata, whatever....
	ble_gatts_char_md_t characteristicMetadata;
	memset(&characteristicMetadata, 0, sizeof(ble_gatts_char_md_t));
	characteristicMetadata.char_props.read = 1; /*Reading value permitted*/
	characteristicMetadata.char_props.write = 1; /*Writing value with Write Request permitted*/
	characteristicMetadata.char_props.write_wo_resp = 1; /*Writing value with Write Command permitted*/
	characteristicMetadata.char_props.auth_signed_wr = 0; /*Writing value with Signed Write Command not permitted*/
	characteristicMetadata.char_props.notify = 1; /*Notications of value permitted*/
	characteristicMetadata.char_props.indicate = 0; /*Indications of value not permitted*/
	characteristicMetadata.p_cccd_md = &clientCharacteristicConfigurationDescriptor;
  characteristicMetadata.p_char_pf = &clientCharacteristicPresentationFormat;

	//Set human readable name
	uint8_t humanReadableCharacteristicDescription[] = "meshWrite";
	characteristicMetadata.p_char_user_desc = humanReadableCharacteristicDescription;
	characteristicMetadata.char_user_desc_max_size = strlen((const char*) humanReadableCharacteristicDescription);
	characteristicMetadata.char_user_desc_size = strlen((const char*) humanReadableCharacteristicDescription);


	//Finally, the attribute
	ble_gatts_attr_t attribute;
	memset(&attribute, 0, sizeof(ble_gatts_attr_t));

	ble_uuid_t attributeUUID;
	attributeUUID.type = meshService.serviceUuid.type;
	attributeUUID.uuid = MESH_SERVICE_CHARACTERISTIC_UUID;

	attribute.p_uuid = &attributeUUID; /* The UUID of the Attribute*/
	attribute.p_attr_md = &attributeMetadata; /* The previously defined attribute Metadata */
	attribute.max_len = MESH_CHARACTERISTIC_MAX_LENGTH;
	attribute.init_len = 0;
	attribute.init_offs = 0;

  attribute.p_value = WATEVER;

	//Finally, add the characteristic
	err = sd_ble_gatts_characteristic_add(meshService.serviceHandle, &characteristicMetadata, &attribute, &meshService.sendMessageCharacteristicHandle);
	APP_ERROR_CHECK(err);

  // And add the descriptor
  ble_uuid_t desc_uuid = {0};
  desc_uuid.uuid = SERVICE_CHARACTERISTIC_DESCRIPTOR_UUID;
  desc_uuid.type = BLE_UUID_TYPE_BLE;

  ble_gatts_attr_md_t desc_md;
  memset(&desc_md, 0, sizeof(ble_gatts_attr_md_t));
  desc_md.vloc = BLE_GATTS_VLOC_STACK;
  desc_md.vlen = 0;
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&desc_md.write_perm);

  ble_gatts_attr_t desc;
  memset(&desc, 0, sizeof(ble_gatts_attr_t));
  desc.p_value = SERVICE_CHARACTERISTIC_DESC_VALUE;
  desc.init_len = sizeof(SERVICE_CHARACTERISTIC_DESC_VALUE);
  desc.max_len = sizeof(SERVICE_CHARACTERISTIC_DESC_VALUE);
  desc.p_uuid = &desc_uuid;
  desc.p_attr_md = &desc_md;

  uint16_t someHandle = 0;
  EC(sd_ble_gatts_descriptor_add(meshService.sendMessageCharacteristicHandle.value_handle, &desc, &someHandle));
}

void write_value(uint16_t connectionHandle, uint8_t *data, uint16_t dataLength)
{
  ble_gattc_write_params_t writeParams;
  memset(&writeParams, 0, sizeof(writeParams));
  writeParams.write_op = BLE_GATT_OP_WRITE_CMD;
  writeParams.handle = meshService.sendMessageCharacteristicHandle.value_handle;
  writeParams.offset = 0;
  writeParams.len = dataLength;
  writeParams.p_value = data;

  EC(sd_ble_gattc_write(connectionHandle, &writeParams));
}


void update_family()
{
  uint8_t connectionCount;
  uint16_t connectionHandles[4];
  get_active_connection_handles(connectionHandles, &connectionCount);
  for (int i = 0; i < connectionCount; i++){
    log("Sending familyId %u to connection handle %u", familyId, connectionHandles[i]);
    write_value(connectionHandles[i], &familyId, sizeof(familyId));
  }
}
