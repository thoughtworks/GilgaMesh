#include <votes.h>
#include <linked_list.h>
#include <sdk_common.h>
#include <timer.h>
#include <gatt.h>
#include <pstorage.h>


static linkedList userVotes = { 0 };
static userVote buffer = { 0 };
static pstorage_handle_t storageHandle;


void get_votes_from_storage(uint8_t *p_data, uint32_t data_len) {
  userVote voteArray[MAX_VOTE_COUNT];
  memset(voteArray, 0, VOTE_STORAGE_SIZE);
  memcpy(voteArray, p_data, data_len);

  for (int i = 0; i < MAX_VOTE_COUNT; i++) {
    userVote oldVote = voteArray[i];
    if (oldVote.voterId == 0) break;
    userVote *newVote = malloc(sizeof(userVote));
    memcpy(newVote, &oldVote, sizeof(userVote));
    add_item_to_list(&userVotes, (uint8_t *) newVote);
  }

  free(p_data);
}


void set_votes_in_storage(pstorage_handle_t *handle) {
  userVote voteArray[MAX_VOTE_COUNT];
  memset(voteArray, 0, VOTE_STORAGE_SIZE);
  listItem *currentItem = userVotes.first;
  int index = 0;
  while (currentItem != NULL) {
    voteArray[index++] = *((userVote *) currentItem->item);
    currentItem = (listItem *) currentItem->nextListItem;
  }
  pstorage_store(handle, (uint8_t *) voteArray, VOTE_STORAGE_SIZE, 0);
}


void storage_event_handler(pstorage_handle_t * p_handle, uint8_t op_code, uint32_t result, uint8_t *p_data, uint32_t data_len) {

  if (op_code == PSTORAGE_CLEAR_OP_CODE && result == NRF_SUCCESS) {
    // we just cleared out storage, so now we save the votes
    set_votes_in_storage(p_handle);

  } else if (op_code == PSTORAGE_LOAD_OP_CODE && result == NRF_SUCCESS) {
    get_votes_from_storage(p_data, data_len);

  }
}


void storage_initialize() {
  EC(pstorage_init());

  pstorage_module_param_t storageParams;
  storageParams.block_count = 1;
  storageParams.block_size = VOTE_STORAGE_SIZE;
  storageParams.cb = storage_event_handler;
  EC(pstorage_register(&storageParams, &storageHandle));
}


void load_current_votes_from_storage() {
  void *storedVotes = malloc(VOTE_STORAGE_SIZE);
  EC(pstorage_load(storedVotes, &storageHandle, VOTE_STORAGE_SIZE, 0));
}


void user_votes_initialize(void) {
  storage_initialize();

  memset(&userVotes, 0, sizeof(userVotes));
  memset(&buffer, 0, sizeof(buffer));
  userVotes.maxSize = MAX_VOTE_COUNT;
  load_current_votes_from_storage();
  turn_on_send_vote_timeout();
}


void persist_current_votes_in_storage() {
  // the clear op will trigger our storage_event_handler, then current votes will be stored
  EC(pstorage_clear(&storageHandle, VOTE_STORAGE_SIZE));
}


bool buffer_is_empty() {
  return buffer.voterId == 0 && buffer.hitCount == 0;
}


bool add_buffer_contents_to_list() {
  if (buffer_is_empty()) return true;

  userVote *newVote = malloc(sizeof(userVote));
  memcpy(newVote, &buffer, sizeof(userVote));
  bool successful = add_item_to_list(&userVotes, (uint8_t *) newVote);
  if (successful) {
    persist_current_votes_in_storage();
    memset(&buffer, 0, sizeof(buffer));
  } else {
    turn_on_buffer_clear_timeout();
    free(newVote);
  }
  return successful;
}


void clear_vote_buffer(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  add_buffer_contents_to_list();
}


bool save_vote(uint16_t voterId) {
  turn_off_buffer_clear_timeout();

  if (buffer.voterId == voterId) {
    buffer.hitCount++;
  } else {
    bool successful = add_buffer_contents_to_list();
    if (!successful) return false;

    buffer.voterId = voterId;
    buffer.hitCount = 1;
  }

  turn_on_buffer_clear_timeout();

  return true;
}


bool remove_vote(userVote *vote) {
  bool successful = remove_item_from_list(&userVotes, (uint8_t *) vote, sizeof(userVote));
  if (successful) {
    persist_current_votes_in_storage();
  }
  return successful;
}


userVote *get_first_vote() {
  return (userVote *) ((userVotes.count == 0) ? NULL : userVotes.first->item);
}


uint16_t get_vote_count() {
  return (uint16_t) (userVotes.count + (buffer_is_empty() ? 0 : 1));
}

