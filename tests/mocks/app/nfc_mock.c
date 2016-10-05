#include "app/nfc.h"
#include "cmocka_includes.h"

bool nfc_initialize(void) {
  return true;
}

void nfc_scan(void) { }

nfcStatus get_nfc_status(void) {
  return mock_type(nfcStatus);
}

const char* get_nfc_status_string(nfcStatus status) {
  return "some status";
}