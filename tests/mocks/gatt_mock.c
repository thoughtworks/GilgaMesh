#include <gatt.h>

#include "cmocka_includes.h"

#if defined(TESTING) && !defined(GATT_TEST)

void gatt_initialize(void) {

}

void write_value(uint16_t connectionHandle, uint8_t *data, uint16_t dataLength) {
   check_expected(connectionHandle);
   check_expected(dataLength);
}

void update_family(void) {

}

void propagate_family_id(uint16_t originHandle) {
   check_expected(originHandle);
}

#endif