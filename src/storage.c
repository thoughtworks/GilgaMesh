#include "storage.h"
#include <string.h>
#include <stdlib.h>
#include <sdk_common.h>
#include <gatt.h>
#include "version.h"

#define STORAGE_BLOCK_SIZE              PSTORAGE_MIN_BLOCK_SIZE

static pstorage_handle_t versionStorageHandle;
static pstorage_handle_t voteConfigStorageHandle;
static pstorage_handle_t voteMetadataStorageHandle;
static pstorage_handle_t voteStorageHandle;


void set_vote_in_storage(userVote *vote, uint16_t index) {
  userVote *voteCopy = memcpy(malloc(sizeof(userVote)), vote, sizeof(userVote));

  pstorage_handle_t currentVoteStorageHandle;
  pstorage_block_identifier_get(&voteStorageHandle, index, &currentVoteStorageHandle);
  pstorage_update(&currentVoteStorageHandle, (uint8_t *) voteCopy, STORAGE_BLOCK_SIZE, 0);
}

void get_vote_from_storage(uint16_t index) {
  pstorage_handle_t currentVoteStorageHandle;
  pstorage_block_identifier_get(&voteStorageHandle, index, &currentVoteStorageHandle);
  pstorage_load(malloc(STORAGE_BLOCK_SIZE), &currentVoteStorageHandle, STORAGE_BLOCK_SIZE, 0);
}

void get_version_from_storage() {
  pstorage_load(malloc(STORAGE_BLOCK_SIZE), &versionStorageHandle, STORAGE_BLOCK_SIZE, 0);
}

void set_version_in_storage(uint8_t *version) {
  pstorage_store(&versionStorageHandle, version, STORAGE_BLOCK_SIZE, 0);
}

void clear_version_in_storage() {
  pstorage_clear(&versionStorageHandle, STORAGE_BLOCK_SIZE);
}

void get_vote_config_from_storage() {
  pstorage_load(malloc(STORAGE_BLOCK_SIZE), &voteConfigStorageHandle, STORAGE_BLOCK_SIZE, 0);
}

void set_vote_config_in_storage(uint8_t *voteConfiguration) {
  pstorage_store(&voteConfigStorageHandle, voteConfiguration, STORAGE_BLOCK_SIZE, 0);
}

void clear_vote_config_in_storage() {
  pstorage_clear(&voteConfigStorageHandle, STORAGE_BLOCK_SIZE);
}

void get_vote_metadata_from_storage() {
  pstorage_load(malloc(STORAGE_BLOCK_SIZE), &voteMetadataStorageHandle, STORAGE_BLOCK_SIZE, 0);
}

void set_vote_metadata_in_storage(voteMetadata *metadata) {
  voteMetadata *metadataCopy = memcpy(malloc(sizeof(voteMetadata)), metadata, sizeof(voteMetadata));
  pstorage_update(&voteMetadataStorageHandle, (uint8_t *) metadataCopy, STORAGE_BLOCK_SIZE, 0);
}

bool version_has_changed(uint8_t *previousVersion) {
  return (previousVersion[0] != APP_VERSION_MAIN) || (previousVersion[1] != APP_VERSION_SUB);
}

uint8_t* build_version_data() {
  uint8_t *versionInfo = malloc(STORAGE_BLOCK_SIZE); //TODO change this to 2 bytes, see what happens
  versionInfo[0] = APP_VERSION_MAIN;
  versionInfo[1] = APP_VERSION_SUB;
  return versionInfo;
}

void check_version_and_load_votes(uint8_t *storedVersion) {
  if (version_has_changed(storedVersion)) {
    clear_version_in_storage();
    clear_vote_config_in_storage();
  } else {
    get_vote_config_from_storage();
    get_vote_metadata_from_storage();
  }
}

void version_storage_event_handler(uint8_t op_code, uint8_t *p_data) {
  if (op_code == PSTORAGE_CLEAR_OP_CODE) {
    set_version_in_storage(build_version_data());
  }
  if (op_code == PSTORAGE_LOAD_OP_CODE) {
    check_version_and_load_votes(p_data);
    free(p_data);
  }
  if (op_code == PSTORAGE_STORE_OP_CODE) {
    free(p_data);
  }
}

void vote_config_storage_event_handler(uint8_t op_code, uint8_t *p_data) {
  if (op_code == PSTORAGE_CLEAR_OP_CODE) {
    set_vote_config_in_storage((uint8_t *) get_vote_configuration());
  }
  if (op_code == PSTORAGE_LOAD_OP_CODE) {
    set_vote_configuration(p_data);
    free(p_data);
  }
}

void vote_metadata_storage_event_handler(uint8_t op_code, uint8_t *p_data) {
  if (op_code == PSTORAGE_LOAD_OP_CODE) {
    set_vote_metadata((voteMetadata *) p_data);
    free(p_data);
  }
  if (op_code == PSTORAGE_UPDATE_OP_CODE) {
    free(p_data);
  }
}

void vote_storage_event_handler(uint8_t op_code, uint8_t *p_data) {
  if (op_code == PSTORAGE_LOAD_OP_CODE) {
    process_vote_from_storage((userVote *) p_data);
    free(p_data);
  }
  if (op_code == PSTORAGE_UPDATE_OP_CODE) {
    free(p_data);
  }
}

void retry_storage_event(pstorage_handle_t *p_handle, uint8_t op_code, uint8_t *p_data, pstorage_size_t data_len) {
  switch(op_code) {
    case PSTORAGE_CLEAR_OP_CODE:
      pstorage_clear(p_handle, data_len);
      break;
    case PSTORAGE_LOAD_OP_CODE:
      pstorage_load(p_data, p_handle, data_len, 0);
      break;
    case PSTORAGE_STORE_OP_CODE:
      pstorage_store(p_handle, p_data, data_len, 0);
      break;
    case PSTORAGE_UPDATE_OP_CODE:
      pstorage_update(p_handle, p_data, data_len, 0);
      break;
    default:
      break;
  }
}

void storage_event_handler(pstorage_handle_t *p_handle, uint8_t op_code, uint32_t result, uint8_t *p_data, uint32_t data_len) {
  if (result != NRF_SUCCESS) {
    retry_storage_event(p_handle, op_code, p_data, (pstorage_size_t) data_len);
  } else {
    if (p_handle->block_id == versionStorageHandle.block_id) {
      version_storage_event_handler(op_code, p_data);
    } else if (p_handle->block_id == voteConfigStorageHandle.block_id) {
      vote_config_storage_event_handler(op_code, p_data);
    } else if (p_handle->block_id == voteMetadataStorageHandle.block_id) {
      vote_metadata_storage_event_handler(op_code, p_data);
    } else {
      vote_storage_event_handler(op_code, p_data);
    }
  }
}

void register_for_storage(pstorage_handle_t *handle, pstorage_size_t blockCount, pstorage_size_t blockSize, pstorage_ntf_cb_t callback) {
  pstorage_module_param_t storageParams;
  storageParams.block_count = blockCount;
  storageParams.block_size = blockSize;
  storageParams.cb = callback;
  EC(pstorage_register(&storageParams, handle));
}

void storage_initialize() {
  EC(pstorage_init());
  register_for_storage(&versionStorageHandle, MAX_VOTE_COUNT + 3, STORAGE_BLOCK_SIZE, storage_event_handler);

  pstorage_block_identifier_get(&versionStorageHandle, 1, &voteConfigStorageHandle);
  pstorage_block_identifier_get(&versionStorageHandle, 2, &voteMetadataStorageHandle);
  pstorage_block_identifier_get(&versionStorageHandle, 3, &voteStorageHandle);
}

void load_version_information() {
  get_version_from_storage();
}

void save_vote_configuration() {
  clear_vote_config_in_storage();
}

