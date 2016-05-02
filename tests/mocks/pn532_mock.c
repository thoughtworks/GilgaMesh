#include "pn532.h"

#include "cmocka_includes.h"

nfcStatus get_nfc_status() {
  return mock_type(nfcStatus);
}
