#include <app_error.h>
#include <system/timer.h>
#include <nfc_t2t_parser.h>
#include <nfc_ndef_msg_parser.h>
#include <stdlib.h>
#include <error.h>

#include "app/nfc.h"
#include "app/pn532/adafruit_pn532.h"
#include "app/votes.h"

static uint8_t tag_data[TAG_DATA_BUFFER_SIZE];
static bool nfcEnabled = false;
static bool nfcErrorCondition = false;
static uint32_t delay_time_left_x100ms = 0;

SYS_TIMER_DEF(nfcScanTimer);

void nfc_submit_vote(uint8_t * url_data, uint32_t data_length)
{
  int url_length = data_length + 1; // reserve space for null termination
  char urlstr[url_length];
  char* id_tag_position;

  memcpy(urlstr, url_data, data_length);
  for(int i = 0 ; i < url_length + 1 ; i++) {
    urlstr[i] == 0 ? urlstr[i] = 0xFF : false; // replace any terminating characters with 0xFF
  }

  urlstr[data_length] = '\0'; // null terminate the url data at the end
  id_tag_position = strstr(urlstr, ID_TAG_STRING);

  if(id_tag_position != NULL) {
    char id_string[NUMBER_OF_ID_DIGITS + 1];
    memcpy(id_string, id_tag_position + ID_TAG_LENGTH, NUMBER_OF_ID_DIGITS);
    id_string[NUMBER_OF_ID_DIGITS] = '\0'; // null terminate the id string

    uint16_t id = atoi(id_string);

    MESH_LOG(COLOR_CODE_BLUE);
    MESH_LOG("Voter id: %d\r\n", id);
    MESH_LOG(COLOR_CODE_DEFAULT);

    save_vote(id);
  }
  else {
    MESH_LOG_WARNING("No voter id found on the NFC tag\r\n");
  }
}

static void after_read_delay(void) {
  ret_code_t err_code;

  // Turn off the RF field.
  err_code = adafruit_pn532_field_off();
  APP_ERROR_CHECK(err_code);

  delay_time_left_x100ms = TAG_AFTER_READ_DELAY;
}

/**
 * @brief Function for reading data from a Tag.
 *
 * This function waits for a Tag to appear in the field. When a Tag is detected, all of the pages
 * within a Tag are read.
 */
static ret_code_t tag_data_read(uint8_t * buffer, uint32_t buffer_size) {
  ret_code_t err_code;

  // Buffer for UID.
  uint8_t uid[TAG_TYPE_2_UID_LENGTH] = { 0 };
  uint8_t uid_length = TAG_TYPE_2_UID_LENGTH;

  // Not enough size in the buffer to read a tag header.
  if (buffer_size < T2T_FIRST_DATA_BLOCK_OFFSET)
  {
    return NRF_ERROR_NO_MEM;
  }

  // Detect a ISO14443A Tag in the field and initiate a communication. This function activates
  // the NFC RF field. If a Tag is present, its UID is stored in the uid buffer. The
  // UID read from the Tag can not be longer than uidLength value passed to the function.
  // As a result, the uidLength variable returns length of the Tags UID read.
  err_code = adafruit_pn532_read_passive_target_id(PN532_MIFARE_ISO14443A_BAUD,
                                                   uid,
                                                   &uid_length,
                                                   TAG_DETECT_TIMEOUT);
  if (err_code != NRF_SUCCESS)
  {
    return NRF_ERROR_NOT_FOUND;
  }

  if (uid_length != TAG_TYPE_2_UID_LENGTH)
  {
    return NRF_ERROR_NOT_SUPPORTED;
  }


  uint8_t cmd_buf[2];
  uint8_t response_len = 16;

  cmd_buf[0] = MIFARE_CMD_READ;
  cmd_buf[1] = 8;

  uint8_t ourBuffer[response_len * 2];
  EC(adafruit_pn532_in_data_exchange(cmd_buf, 2, ourBuffer, &response_len));
  cmd_buf[1] = 12;
  EC(adafruit_pn532_in_data_exchange(cmd_buf, 2, ourBuffer + response_len, &response_len));

  MESH_LOG("Read %u bytes: %s\r\n", response_len*2, ourBuffer);

  memcpy(buffer, ourBuffer, 32);

  return NRF_SUCCESS;
}

void nfc_scan(void) {
  uint32_t err_code;

  if(delay_time_left_x100ms != 0) {
    delay_time_left_x100ms--;
  }
  else {
    if (nfcEnabled && !nfcErrorCondition && adafruit_pn532_is_ready()) {
      err_code = tag_data_read(tag_data, TAG_DATA_BUFFER_SIZE);
      switch (err_code) {
        case NRF_SUCCESS:
        MESH_LOG_INFO("NFC tag present.\r\n");
          nfc_submit_vote(tag_data, TAG_DATA_BUFFER_SIZE);
          after_read_delay();
          break;
        case NRF_ERROR_NO_MEM:
          MESH_LOG_WARNING("Declared buffer is too small to store tag data.\r\n");
          after_read_delay();
          break;
        case NRF_ERROR_NOT_FOUND:
        MESH_LOG_DEBUG("No Tag found.\r\n");
          // No delay here as we want to search for another tag immediately.
          break;
        case NRF_ERROR_NOT_SUPPORTED:
          MESH_LOG_WARNING("Tag not supported.\r\n");
          after_read_delay();
          break;
        default:
          MESH_LOG_WARNING("Tag removed.\r\n");
          adafruit_pn532_field_off();
          break;
      }
    }
  }
}

nfcStatus get_nfc_status(void) {
  if (!nfcEnabled) return NFC_STATUS_DISABLED;
  if (nfcErrorCondition) return NFC_STATUS_ERROR;
  return NFC_STATUS_WORKING;
}

const char* get_nfc_status_string(nfcStatus status) {
  if (status == NFC_STATUS_DISABLED)  return "DISABLED";
  if (status == NFC_STATUS_ERROR)     return "ERROR";
  return "ENABLED";
}

void nfc_initialize(void) {
  uint32_t err_code;
  err_code = adafruit_pn532_init(false);
  APP_ERROR_CHECK(err_code);

  if(!err_code) {
    nfcEnabled = true;

    create_repeated_timer(&nfcScanTimer);
    start_timer(&nfcScanTimer, NFC_SCAN_FREQUENCY_IN_MS, nfc_scan);
  } else {
    nfcErrorCondition = true;
  }
}