#include <app_error.h>
#include <system/timer.h>
#include <nfc_t2t_parser.h>
#include <nfc_ndef_msg_parser.h>
#include <system/delay.h>
#include <stdlib.h>

#include "app/nfc.h"
#include "app/pn532/adafruit_pn532.h"
#include "app/votes.h"

static uint8_t tag_data[TAG_DATA_BUFFER_SIZE];
static bool nfcEnabled = false;
static bool nfcErrorCondition = false;
static uint32_t delay_time_left_x100ms = 0;

static char const * const nfc_tnf_descs[] =
        {
                "Empty\r\n",
                "NFC Forum well-known type\r\n",
                "Media-type (RFC 2046)\r\n",
                "Absolute URI (RFC 3986)\r\n",
                "NFC Forum external type (NFC RTD)\r\n",
                "Unknown\r\n",
                "Unchanged\r\n",
                "Reserved\r\n"
        };

SYS_TIMER_DEF(nfcScanTimer);

static void nfc_ndef_record_printout(uint32_t num, nfc_ndef_record_desc_t * const p_rec_desc)
{
  MESH_LOG_INFO("NDEF record %d content:\r\n", num);

  if (p_rec_desc->p_id != NULL)
  {
    MESH_LOG_INFO("ID:\r\n");
    MESH_LOG_HEXDUMP_INFO((uint8_t *)p_rec_desc->p_id, p_rec_desc->id_length);
  }

  if (p_rec_desc->p_type != NULL)
  {
    MESH_LOG_INFO("type:\r\n");
    MESH_LOG_HEXDUMP_INFO((uint8_t *)p_rec_desc->p_type, p_rec_desc->type_length);
  }

  if (p_rec_desc->payload_constructor == (p_payload_constructor_t) nfc_ndef_bin_payload_memcopy)
  {
    nfc_ndef_bin_payload_desc_t * p_bin_pay_desc = p_rec_desc->p_payload_descriptor;

    if (p_bin_pay_desc->p_payload != NULL)
    {
      MESH_LOG_INFO("Payload data (%d bytes):\r\n", p_bin_pay_desc->payload_length);
      MESH_LOG_HEXDUMP_INFO((uint8_t *)p_bin_pay_desc->p_payload, p_bin_pay_desc->payload_length);
    }
    else
    {
      MESH_LOG_INFO("No payload\r\n");
    }
  }
  MESH_LOG_INFO("\r\n\r\n");
}

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

void nfc_ndef_msg_to_vote(nfc_ndef_msg_desc_t * const p_msg_desc)
{
  uint32_t i;

  system_delay_ms(100);
  MESH_LOG_DEBUG("NDEF message contains %d record(s)\r\n\r\n", p_msg_desc->record_count);

  for (i = 0; i < p_msg_desc->record_count; i++)
  {
    //ndef_record_printout(i, p_msg_desc->pp_record[i]);

    MESH_LOG_DEBUG("NDEF record %d content:\r\n", i);
    MESH_LOG_DEBUG("TNF: %s",(uint32_t)nfc_tnf_descs[p_msg_desc->pp_record[i]->tnf]);

    if (p_msg_desc->pp_record[i]->payload_constructor == (p_payload_constructor_t) nfc_ndef_bin_payload_memcopy)
    {
      nfc_ndef_bin_payload_desc_t * p_bin_pay_desc = p_msg_desc->pp_record[i]->p_payload_descriptor;

      if (p_bin_pay_desc->p_payload != NULL)
      {
        MESH_LOG(COLOR_CODE_GREEN);
        MESH_LOG("Payload data (%d bytes): \r\n", p_bin_pay_desc->payload_length);
        MESH_LOG_HEXDUMP((uint8_t *)(p_bin_pay_desc->p_payload), p_bin_pay_desc->payload_length);
        MESH_LOG(COLOR_CODE_DEFAULT);
        MESH_LOG("\r\n");

        nfc_submit_vote((uint8_t *)(p_bin_pay_desc->p_payload), p_bin_pay_desc->payload_length);
      }
      else
      {
        MESH_LOG_INFO("No NFC tag payload\r\n");
      }
    }
  }
}

static void after_read_delay(void)
{
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
static ret_code_t tag_data_read(uint8_t * buffer, uint32_t buffer_size)
{
  ret_code_t err_code;
  uint8_t i;

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

/**
 * @brief Function for analyzing NDEF data from a TLV block.
 *
 * This function checks if a TLV block is in NDEF format. If an NDEF block is detected,
 * the NDEF data is parsed and printed.
 */
static void ndef_data_analyze(tlv_block_t * p_tlv_block)
{
  uint8_t desc_buf[NFC_NDEF_PARSER_REQIRED_MEMO_SIZE_CALC(10)];
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

    nfc_ndef_msg_to_vote((nfc_ndef_msg_desc_t *) desc_buf);
  }
}

/**
 * @brief Function for analyzing data from a Tag.
 *
 * This function parses content of a Tag and prints it out.
 */
static void tag_data_analyze(uint8_t * buffer)
{
  ret_code_t err_code;

  // Static declaration of Type 2 Tag structure. Maximum of 10 TLV blocks can be read.
  NFC_TYPE_2_TAG_DESC_DEF(test_1, 10);
  type_2_tag_t * test_type_2_tag = &NFC_TYPE_2_TAG_DESC(test_1);

  err_code = type_2_tag_parse(test_type_2_tag, buffer);
  if (err_code == NRF_ERROR_NO_MEM)
  {
    MESH_LOG_WARNING("Not enough memory to read whole tag. Printing what've been read.\r\n\r\n");
  }
  else if (err_code != NRF_SUCCESS)
  {
    MESH_LOG_ERROR("Error during parsing a tag. Printing what could've been read.\r\n\r\n");
  }

  //type_2_tag_printout(test_type_2_tag);

  tlv_block_t * p_tlv_block = test_type_2_tag->p_tlv_block_array;
  uint32_t i;

  MESH_LOG_DEBUG("The tag has %d TLV blocks\r\n", test_type_2_tag->tlv_count);
  for (i = 0; i < test_type_2_tag->tlv_count; i++)
  {
    ndef_data_analyze(p_tlv_block);
    p_tlv_block++;
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
          tag_data_analyze(tag_data);
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