#include <app_error.h>
#include <system/timer.h>
#include <nfc_t2t_parser.h>
#include <nfc_ndef_msg_parser.h>
#include <stdlib.h>
#include <error.h>

#include "app/nfc.h"
#include "app/pn532/adafruit_pn532.h"
#include "app/votes.h"

#include "terminal.h"

static uint8_t tag_data[TAG_DATA_BUFFER_SIZE];
static bool nfcEnabled = false;
static bool nfcErrorCondition = false;
static uint32_t delay_time_left_x100ms = 0;

SYS_TIMER_DEF(nfcScanTimer);

void nfc_submit_vote(uint8_t * url_string, uint32_t url_length)
{
  char* id_tag_position;
  id_tag_position = strstr(url_string, ID_TAG_STRING);

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

static void after_read_delay(uint32_t scans_to_skip) {
  ret_code_t err_code;

  // Turn off the RF field.
  err_code = adafruit_pn532_field_off();
  APP_ERROR_CHECK(err_code);

  delay_time_left_x100ms = scans_to_skip;
}

/**
 * @brief Function for reading data from a Tag.
 *
 * This function waits for a Tag to appear in the field. When a Tag is detected, all of the pages
 * within a Tag are read.
 */
static ret_code_t tag_data_read(uint8_t * buffer, uint32_t buffer_size) {
  memset(buffer, 0, buffer_size);
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
  uint8_t bytesPerRead = 16;
  uint8_t pageSize = 4;
  uint8_t numberOfHeaderPages = 4;
  uint8_t numberOfReads = 4;

  cmd_buf[0] = MIFARE_CMD_READ;

  for (int i = 0; i < numberOfReads; i++) {
    cmd_buf[1] = (pageSize * i) + numberOfHeaderPages;
    EC(adafruit_pn532_in_data_exchange(cmd_buf, 2, buffer + (bytesPerRead * i), &bytesPerRead));
  }

  return NRF_SUCCESS;
}

void nfc_ndef_to_command(nfc_ndef_msg_desc_t * const p_msg_desc) {
  uint32_t i;

  if (p_msg_desc->record_count > 1) {
    MESH_LOG_DEBUG("%d command record(s) found\r\n", p_msg_desc->record_count - 1);

    for (i = 1; i < p_msg_desc->record_count; i++) // skip the command header record
    {
      if (p_msg_desc->pp_record[i]->payload_constructor == (p_payload_constructor_t) nfc_ndef_bin_payload_memcopy) {
        nfc_ndef_bin_payload_desc_t *p_bin_pay_desc = p_msg_desc->pp_record[i]->p_payload_descriptor;

        if (p_bin_pay_desc->p_payload != NULL) {
          uint32_t command_length = p_bin_pay_desc->payload_length - 3;
          const uint8_t* command_position = p_bin_pay_desc->p_payload + 3;
          char command_string[command_length + 1];
          memcpy(&command_string, command_position, command_length);
          command_string[command_length] = '\0';
          char* end_position = strstr(command_string, COMMAND_END_STRING);

          if (end_position != NULL) {
            memset(end_position, 0, 1); // replace the 1st character of the command end string with 0x0

            MESH_LOG(COLOR_CODE_GREEN);
            MESH_LOG("Executing NFC command: %s\r\n", command_string);
            MESH_LOG(COLOR_CODE_DEFAULT);
            MESH_LOG("\r\n");

            terminal_execute_command(command_string);

          } else {
            MESH_LOG_WARNING("No terminated command string found on record %d\r\n", i);
          }
        }
        else {
          MESH_LOG_INFO("No NDEF command tag payload\r\n");
        }
      }
    }
  }
  else {
    MESH_LOG_WARNING("No command records found\r\n");
  }
}

static void ndef_data_analyze(tlv_block_t * p_tlv_block)
{
  uint8_t desc_buf[NFC_NDEF_PARSER_REQIRED_MEMO_SIZE_CALC(MAX_COMMAND_TAG_TLV_BLOCKS)];
  uint32_t nfc_data_len;
  uint32_t desc_buf_len = sizeof(desc_buf);

  ret_code_t ret_code;

  if (p_tlv_block->tag == TLV_NDEF_MESSAGE)
  {
    nfc_data_len = p_tlv_block->length;

    ret_code = ndef_msg_parser(desc_buf,
                               &desc_buf_len,
                               p_tlv_block->p_value,
                               &nfc_data_len);

    if (ret_code != NRF_SUCCESS)
    {
      MESH_LOG_ERROR("Error during parsing a NDEF message.\r\n");
    }

    nfc_ndef_to_command((nfc_ndef_msg_desc_t *) desc_buf);
  }
}

static ret_code_t nfc_command_tag_read_rest(uint8_t * buffer, uint32_t buffer_size)
{
  ret_code_t err_code;
  uint8_t i;

  // Read pages 0 - 3 to get the header information.
  for (i = 0; i < 4; i++)
  {
    err_code = adafruit_pn532_ntag2xx_read_page(i, buffer + 4 * i);
    if (err_code)
    {
      MESH_LOG_WARNING("Failed to read page %d\r\n", i);
      return NRF_ERROR_INTERNAL;
    }
  }

  uint16_t data_bytes_in_tag = TAG_TYPE_2_DATA_AREA_MULTIPLICATOR *
                               buffer[TAG_TYPE_2_DATA_AREA_SIZE_OFFSET];

  if (data_bytes_in_tag + T2T_FIRST_DATA_BLOCK_OFFSET > buffer_size)
  {
    //TODO: handle errors returned from this function!!
    return NRF_ERROR_NO_MEM;
  }

  uint8_t pages_to_read = data_bytes_in_tag / T2T_BLOCK_SIZE;
  for (i = 0; i < pages_to_read; i++)
  {
    uint16_t offset_for_page = T2T_FIRST_DATA_BLOCK_OFFSET + 4 * i;
    err_code = adafruit_pn532_ntag2xx_read_page(i + 4, buffer + offset_for_page);
    if (err_code)
    {
      MESH_LOG_WARNING("Failed to read page %d\r\n", i + 4);
      return NRF_ERROR_INTERNAL;
    }
  }

  return NRF_SUCCESS;
}

static void nfc_process_command_tag(uint8_t* buffer, uint32_t data_size) {
  ret_code_t err_code;

  NFC_TYPE_2_TAG_DESC_DEF(test_1, MAX_COMMAND_TAG_TLV_BLOCKS);
  type_2_tag_t * test_type_2_tag = &NFC_TYPE_2_TAG_DESC(test_1);

  err_code = type_2_tag_parse(test_type_2_tag, buffer);
  if (err_code == NRF_ERROR_NO_MEM)
  {
    MESH_LOG_WARNING("Not enough memory to read whole tag.\r\n");
  }
  else if (err_code != NRF_SUCCESS)
  {
    MESH_LOG_ERROR("Error during parsing a tag: %u\r\n", err_code);
  }

  tlv_block_t * p_tlv_block = test_type_2_tag->p_tlv_block_array;
  uint32_t i;

  MESH_LOG_DEBUG("The tag has %d TLV blocks\r\n", test_type_2_tag->tlv_count);
  for (i = 0; i < test_type_2_tag->tlv_count; i++)
  {
    ndef_data_analyze(p_tlv_block);
    p_tlv_block++;
  }
}

static void nfc_process_tag(uint8_t* tag_data, uint32_t data_size) {
  int url_length = data_size + 1; // reserve space for null termination
  char urlstr[url_length];

  memcpy(urlstr, tag_data, data_size);
  for(int i = 0 ; i < url_length + 1 ; i++) {
    if (urlstr[i] == 0) urlstr[i] = 0xFF; // replace any terminating characters with 0xFF
  }

  urlstr[url_length] = '\0'; // null terminate the url data at the end

  if(strstr(urlstr, COMMAND_TAG_STRING) != NULL) {
    MESH_LOG("Command tag detected. Reading NDEF data.\r\n");
    nfc_command_tag_read_rest(tag_data, data_size); // command tag, read it fully
    nfc_process_command_tag(tag_data, data_size);
    after_read_delay(TAG_AFTER_COMMAND_DELAY);
  }
  else { // accelerated (non-NDEF compliant) vote processing forks from here
    nfc_submit_vote(urlstr, url_length);
    after_read_delay(TAG_AFTER_VOTE_DELAY);
  }
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
          nfc_process_tag(tag_data, TAG_DATA_BUFFER_SIZE);
          break;
        case NRF_ERROR_NO_MEM:
          MESH_LOG_WARNING("Declared buffer is too small to store tag data.\r\n");
          after_read_delay(TAG_AFTER_ERROR_DELAY);
          break;
        case NRF_ERROR_NOT_FOUND:
        MESH_LOG_DEBUG("No Tag found.\r\n");
          // No delay here as we want to search for another tag immediately.
          break;
        case NRF_ERROR_NOT_SUPPORTED:
          MESH_LOG_WARNING("Tag not supported.\r\n");
          after_read_delay(TAG_AFTER_ERROR_DELAY);
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

bool nfc_initialize(void) {
  uint32_t err_code;
  err_code = adafruit_pn532_init(false);

  if(execute_succeeds(err_code)) {
    if(!execute_succeeds(create_repeated_timer(&nfcScanTimer))) return nfcEnabled;
    if(!execute_succeeds(start_timer(&nfcScanTimer, NFC_SCAN_FREQUENCY_IN_MS, nfc_scan))) return nfcEnabled;
    nfcEnabled = true;
  } else {
    nfcErrorCondition = true;
  }

  return nfcEnabled;
}