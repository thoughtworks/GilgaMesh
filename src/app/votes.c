#include <votes.h>
#include <sdk_common.h>
#include <timer.h>
#include <gatt.h>
#include <storage.h>
#include <app_scheduler.h>
#include <feedback.h>
#include <limits.h>


static voteMetadata votes = { 0, 0 };
static userVote buffer = { 0 };
static userVoteAck voteAckBuffer = { 0, 0 };


void set_vote_metadata(voteMetadata *metadata) {
  if (metadata->count <= MAX_VOTE_COUNT) { // otherwise, something is wrong and we start from 0
    votes = *metadata;
    log("Loaded %u votes from persistent storage", votes.count);
  }
}

bool buffer_is_empty() {
  return buffer.voterId == 0 && buffer.hitCount == 0;
}

void increment_vote_count() {
  votes.count++;
  set_vote_metadata_in_storage(&votes);
}

void decrement_vote_count() {
  votes.count--;
  votes.startIndex = (uint16_t) ((votes.startIndex + 1) % MAX_VOTE_COUNT);
  set_vote_metadata_in_storage(&votes);
}

uint16_t get_vote_insertion_index() {
  return (uint16_t) ((votes.count + votes.startIndex) % MAX_VOTE_COUNT);
}

bool add_buffer_contents_to_storage() {
  if (buffer_is_empty()) return true;

  if (votes.count >= MAX_VOTE_COUNT) {
    turn_on_buffer_clear_timeout();
    return false;
  }

  set_vote_in_storage(&buffer, get_vote_insertion_index());
  increment_vote_count();
  memset(&buffer, 0, sizeof(buffer));

  return true;
}

void clear_vote_buffer(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  add_buffer_contents_to_storage();
}

void save_vote(uint16_t voterId) {
  turn_off_buffer_clear_timeout();

  if (buffer.voterId == voterId) {
    buffer.hitCount++;
  } else {
    bool successful = add_buffer_contents_to_storage();
    if (!successful) return;

    buffer.voterId = voterId;
    buffer.hitCount = 1;
    buffer.config = *get_vote_configuration();

    display_vote_recorded_feedback();
  }
  rtc_get_timestamp(buffer.timeOfLastVote);

  turn_on_buffer_clear_timeout();
}

void get_first_vote(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  if (votes.count > 0) {
    get_vote_from_storage(votes.startIndex);
  }
}

bool acknowledgement_buffer_matches_vote(userVote *vote) {
  if (voteAckBuffer.voterId != vote->voterId) return false;
  if (!rtc_is_equal_timestamp(voteAckBuffer.timestamp, vote->timeOfLastVote)) return false;

  return true;
}

void process_vote_from_storage(userVote *vote) {
  if (vote->voterId == USHRT_MAX) return; // we tried to access storage before the vote was saved. whoops.

  if (voteAckBuffer.voterId != 0) {
    turn_off_send_vote_timeout();
    if (acknowledgement_buffer_matches_vote(vote)) decrement_vote_count();
    memset(&voteAckBuffer, 0, sizeof(voteAckBuffer));
    get_first_vote(NULL, 0);
    turn_on_send_vote_timeout();
  } else {
    app_sched_event_put(vote, sizeof(userVote), broadcast_vote);
  }
}

uint16_t get_vote_count() {
  return (uint16_t) (votes.count + (buffer_is_empty() ? 0 : 1));
}

bool vote_storage_is_full() {
#ifndef PROD_BOARD
  return false;
#endif
  return get_vote_count() == MAX_VOTE_COUNT + 1;
}

void user_votes_initialize(void) {
  storage_initialize();

  memset(&votes, 0, sizeof(votes));
  memset(&buffer, 0, sizeof(buffer));
  memset(&voteAckBuffer, 0, sizeof(voteAckBuffer));
  load_version_information();
  turn_on_send_vote_timeout();
}

void log_vote(BleMessageVoteReq *request) {
  timestring_t timestring;

  char votingNodeName[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->deviceId, votingNodeName);

  rtc_timestamp_to_timestring(request->vote.timeOfLastVote, timestring);

  log("VOTE: {\"id\": \"%s\", \"rawId\": %u, \"voterId\": \"%u\", \"hitCount\": %u, \"group\": %u, \"value\": %u, \"timestamp\": \"%s\"}",
      votingNodeName, request->deviceId, request->vote.voterId, request->vote.hitCount, request->vote.config.group,
      request->vote.config.value, timestring);
}


void broadcast_vote(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(dataLength);
  userVote *voteToSend = data;

  BleMessageVoteReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Vote;
  request.deviceId = deviceId;
  request.vote = *voteToSend;

  log_vote(&request);
  send_to_central_connection(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(BleMessageVoteReq));
}


MessagePropagationType receive_vote(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  log_vote((BleMessageVoteReq *)dataPacket);
  return PropagateToCentral;
}

void broadcast_vote_acknowledgement(char* nodeIdStr, char* voterIdStr, timestring_t timestampStr) {
  BleMessageVoteAckReq request;

  memset(&request, 0, sizeof(request));
  request.head.messageType = VoteAcknowledgement;
  request.deviceId = (uint32_t) atoll(nodeIdStr);
  request.voteAck.voterId = (uint16_t) atoi(voterIdStr);
  rtc_timestring_to_timestamp(timestampStr, request.voteAck.timestamp);

  app_sched_event_put(&request, sizeof(BleMessageVoteAckReq), scheduled_broadcast_request);
}

MessagePropagationType receive_vote_acknowledgement(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  BleMessageVoteAckReq *voteAckReq = (BleMessageVoteAckReq *) dataPacket;
  if (voteAckReq->deviceId == deviceId) {
    voteAckBuffer = voteAckReq->voteAck;
    get_first_vote(NULL, 0);
    return DoNotPropagate;
  }
  return PropagateToAll;
}