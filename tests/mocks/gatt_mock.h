#pragma once

void gatt_initialize(void) {

}

void write_value(uint16_t connectionHandle, uint8_t *data, uint16_t dataLength) {
   check_expected(connectionHandle);
   check_expected(dataLength);
}

void update_family(void) {

}
