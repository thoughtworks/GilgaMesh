#include <app_error.h>
#include "app/nfc.h"
#include "app/pn532/adafruit_pn532.h"

static uint8_t tag_data[TAG_DATA_BUFFER_SIZE];
static bool nfcEnabled = false;

void nfc_initialize(void) {
  APP_ERROR_CHECK(adafruit_pn532_init(false));
  nfcEnabled = true;
}

void nfc_scan(void) {

}

nfcStatus get_nfc_status(void) {
  if (!nfcEnabled) return NFC_STATUS_DISABLED;
  // if nfc isn't working, return NFC_STATUS_ERROR
  return NFC_STATUS_WORKING;
}

const char* get_nfc_status_string(nfcStatus status) {
  if (status == NFC_STATUS_DISABLED)  return "DISABLED";
  if (status == NFC_STATUS_ERROR)     return "ERROR";
  return "ENABLED";
}