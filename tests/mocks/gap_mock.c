#include <gap.h>

#include "cmocka_includes.h"

#if defined(TESTING) && !defined(GAP_TEST)

void ble_initialize(void) {

}

void start_advertising(void) {

}

void start_scanning(void) {

}

void handle_gap_event(ble_evt_t * bleEvent) {
   check_expected_ptr(bleEvent);
}

#endif