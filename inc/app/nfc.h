#pragma once

#define PN532_CONFIG_TWI_INSTANCE 1
#define PN532_CHIP_RESET          8
#define PN532_IRQ                 9
#define PN532_CONFIG_SCL          10
#define PN532_CONFIG_SDA          11
#define PN532_RESET               20 // this is a pn532 reset indicator pin

#define NFC_SCAN_FREQUENCY_IN_MS 1000

#define TAG_TYPE_2_UID_LENGTH               7                             /// Length of the Tag's UID.
#define TAG_DATA_BUFFER_SIZE                512                           /// Buffer size for data from a Tag.
#define TAG_DETECT_TIMEOUT                  30                           /// Timeout for function which searches for a tag.
#define TAG_AFTER_READ_DELAY                1                            /// Number of scan cycles to skip after read (in multiples of NFC_SCAN_FREQUENCY_IN_MS)
#define TAG_TYPE_2_DATA_AREA_SIZE_OFFSET    T2T_CC_BLOCK_OFFSET + 2       /// Offset of the byte with Tag's Data size.
#define TAG_TYPE_2_DATA_AREA_MULTIPLICATOR  8                             /// Multiplicator for a value stored in the Tag's Data size byte.


typedef enum {
  NFC_STATUS_DISABLED,
  NFC_STATUS_WORKING,
  NFC_STATUS_ERROR
} nfcStatus;


void nfc_initialize(void);
void nfc_scan(void);
nfcStatus get_nfc_status(void);
const char* get_nfc_status_string(nfcStatus status);