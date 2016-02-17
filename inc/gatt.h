#pragma once

#include <ble.h>
#include <ble_gap.h>

#include <connection.h>
#include <error.h>
#include <logger.h>

// FOO_SERVICE_UUID: 12ec7140-cec6-11e5-b3ae-0002a5d5c51b
#define FOO_SERVICE_UUID               0x1B, 0xC5, 0xD5, 0xA5, 0x02, 0x00, 0xAE, 0xB3, 0xE5, 0x11, 0xC6, 0xCE, 0x40, 0x71, 0xEC, 0x12
#define FOO_CHARACTERISTIC_UUID        0x1524
#define FOO_CHARACTERISTIC_MAX_LENGTH  4
#define FOO_CHARACTERISTIC_DESCRIPTION "meshData"


typedef struct
{
  uint8_t length1;
//  uint32_t newFamilyId;
//  uint16_t length2:4;
  uint8_t newFamilyIdParts[4];
} familyUpdate;

typedef struct
{
	uint16_t                     		serviceHandle;
	ble_gatts_char_handles_t		testValCharacteristicHandle;
	ble_gatts_char_handles_t		sendMessageCharacteristicHandle;
	uint16_t								sendMessageCharacteristicDescriptorHandle;
	ble_uuid_t						serviceUuid;
	uint16_t								connectionHandle;  // Holds the current connection handle (can only be one at a time)
	bool							isNotifying;
} meshServiceStruct;

meshServiceStruct meshService;


void gatt_initialize(void);
void write_value(uint16_t connectionHandle, uint8_t *data, uint16_t dataLength);
void update_family(void);
