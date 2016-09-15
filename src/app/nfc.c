#include <app_error.h>
#include "app/nfc.h"
#include "app/pn532/adafruit_pn532.h"

static uint8_t tag_data[TAG_DATA_BUFFER_SIZE];

void nfc_initialize(void) {
    ret_code_t err_code;

    err_code = adafruit_pn532_init(false);
    APP_ERROR_CHECK(err_code);
}

void nfc_scan(void) {

}