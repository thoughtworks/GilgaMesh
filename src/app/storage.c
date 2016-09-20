#include "app/storage.h"

#include <stddef.h>
#include <fds.h>
#include "system/log.h"

static void fds_evt_handler(fds_evt_t const * const p_fds_evt) {
  switch (p_fds_evt->id) {
    case FDS_EVT_INIT:
      if (p_fds_evt->result != FDS_SUCCESS) MESH_LOG("ERROR: Flash storage initialization failed!\r\n");
      break;
    default:
      break;
  }
}

void storage_initialize() {
  ret_code_t ret = fds_register(fds_evt_handler);
  if (ret != FDS_SUCCESS) MESH_LOG("ERROR: Flash storage callback registration failed!\r\n");

  ret = fds_init();
  if (ret != FDS_SUCCESS) MESH_LOG("ERROR: Flash storage initialization failed!\r\n");
}

static uint16_t get_length_in_words(uint16_t dataLength) {
  uint16_t lengthInWords = (uint16_t) (dataLength / 4);
  if (dataLength % 4 > 0) lengthInWords++;
  return lengthInWords;
}

static void garbage_collect_if_storage_is_limited() {
  fds_stat_t fds_statistics;
  fds_stat(&fds_statistics);
  uint16_t total_words_in_storage = (FDS_VIRTUAL_PAGES - 1) * FDS_VIRTUAL_PAGE_SIZE;
  double percent_words_free = (double)(total_words_in_storage - fds_statistics.words_used) / (double)total_words_in_storage;
  if( percent_words_free <= .05) {
    fds_gc();
  }
}

void check_fds_stats() {
  fds_stat_t fds_statistics;
  fds_stat(&fds_statistics);
  MESH_LOG("total words: %u\r\n", ((FDS_VIRTUAL_PAGES -1) * FDS_VIRTUAL_PAGE_SIZE));
  MESH_LOG("words used: %u\r\n", fds_statistics.words_used);
  MESH_LOG("dirty records: %u\r\n", fds_statistics.dirty_records);
  MESH_LOG("valid records: %u\r\n", fds_statistics.valid_records);
  MESH_LOG("open records: %u\r\n", fds_statistics.open_records);
  MESH_LOG("freeable words: %u\r\n", fds_statistics.freeable_words);
}

void whats_in_storage_right_now() {
  fds_record_desc_t record_desc;
  fds_find_token_t p_token = { 0 };
  while(fds_record_iterate(&record_desc, &p_token) == FDS_SUCCESS) {
    fds_flash_record_t flash_record;
    fds_record_open(&record_desc, &flash_record);
  }
}

void set_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey) {
  fds_record_chunk_t record_chunk = {
          .p_data       = data,
          .length_words = get_length_in_words(dataLength)
  };

  fds_record_t record = {
          .file_id         = fileId,
          .key             = recordKey,
          .data.p_chunks   = &record_chunk,
          .data.num_chunks = 1,
  };

  fds_record_desc_t record_desc;
  ret_code_t result = fds_record_write(&record_desc, &record);
  if (result != FDS_SUCCESS) {
    MESH_LOG("ERROR: Writing data to storage failed!\r\n");
  }
  garbage_collect_if_storage_is_limited();
}

void delete_data_from_storage(uint16_t fileId, uint16_t recordKey) {
  fds_record_desc_t record_desc;
  fds_find_token_t ftok = { 0 };
  if (fds_record_find(fileId, recordKey, &record_desc, &ftok) == FDS_SUCCESS) {
    if (fds_record_delete(&record_desc) != FDS_SUCCESS) {
      MESH_LOG("ERROR: Deleting data from storage failed!\r\n");
    }
  }
  garbage_collect_if_storage_is_limited();
}

void delete_file_from_storage(uint16_t fileId) {
  if(fds_file_delete(fileId) == NRF_SUCCESS) {
    fds_gc();
  }
};

void* get_data_from_storage(uint16_t fileId, uint16_t recordKey) {
  void *result = NULL;
  fds_record_desc_t record_desc;
  fds_find_token_t ftok = { 0 };

  ret_code_t findResult;
  if (recordKey == NULL) {
    findResult = fds_record_find_in_file(fileId, &record_desc, &ftok);
  } else {
    findResult = fds_record_find(fileId, recordKey, &record_desc, &ftok);
  }

  if (findResult == FDS_SUCCESS) {
    fds_flash_record_t flash_record;
    if (fds_record_open(&record_desc, &flash_record) == FDS_SUCCESS) {
      result = (void *) flash_record.p_data;
      fds_record_close(&record_desc);
    } else {
      MESH_LOG("ERROR: Can't open data in storage!\r\n");
    }
  }
  return result;
}

uint16_t get_record_count_in_storage(uint16_t fileId) {
  uint16_t count = 0;
  fds_record_desc_t record_desc;
  fds_find_token_t ftok = { 0 };

  while (fds_record_find_in_file(fileId, &record_desc, &ftok) == FDS_SUCCESS) {
    count++;
  }
  return count;
}

void update_data_in_storage(void *data, uint16_t dataLength, uint16_t fileId, uint16_t recordKey) {
  delete_data_from_storage(fileId, recordKey);
  set_data_in_storage(data, dataLength, fileId, recordKey);
}

