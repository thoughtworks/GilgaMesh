#include "app/custom_message_types/voting_heartbeat_message.h"

#include <ble_types.h>
#include <string.h>

#include "app/votes.h"
#include "system/log.h"
#include "system/timer.h"
#include "system/util.h"
#include "device.h"
#include "gatt.h"
#include "terminal.h"


SYS_TIMER_DEF(votingHeartbeatTimer);

static BleMessageType votingHeartbeatMessageType;

void voting_heartbeat_message_initialize() {
  votingHeartbeatMessageType = register_message_type(receive_voting_heartbeat_message);

  create_repeated_timer(&votingHeartbeatTimer);
  start_timer(&votingHeartbeatTimer, VOTING_MESSAGE_HEARTBEAT_FREQUENCY_IN_MS, send_voting_heartbeat_message);
}

static void log_voting_heartbeat_info(BleMessageVotingHeartbeatReq *request) {
  char deviceId[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->deviceId, deviceId);

  char logMessage[256];
  sprintf(logMessage, "VOTING HB: {\"id\": \"%s\", \"rawId\": %u, \"nfcStatus\": \"%s\", \"group\": %u, \"value\": %u, \"voteCount\": %u}\r\n",
          deviceId, (unsigned int) request->deviceId, get_nfc_status_string(request->nfcStatus), request->voteConfig.group, request->voteConfig.value, request->voteCount);
  MESH_LOG("%s", logMessage);
  log_to_uart(logMessage, sizeof(logMessage));
}

void send_voting_heartbeat_message() {
  BleMessageVotingHeartbeatReq request;
  memset(&request, 0, sizeof(request));
  request.messageType = votingHeartbeatMessageType;
  request.deviceId = get_raw_device_id();
  request.nfcStatus = get_nfc_status();
  request.voteCount = get_vote_count();
  get_vote_configuration(&request.voteConfig);

  log_voting_heartbeat_info(&request);
  send_to_central_connection(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(request));
}

MessagePropagationType receive_voting_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  SYS_UNUSED_PARAMETER(connectionHandle);
  log_voting_heartbeat_info((BleMessageVotingHeartbeatReq *)dataPacket);
  return PropagateToCentral;
}

