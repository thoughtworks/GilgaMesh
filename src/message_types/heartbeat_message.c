#include "message_types/heartbeat_message.h"
#include <stdlib.h>
#include <app_timer.h>
#include <app_scheduler.h>
#include "conversion.h"
#include "device.h"
#include "gatt.h"
#include "system/timer.h"
#include "version.h"

#define MS_RATE_TO_BROADCAST_HEARTBEAT 3000

APP_TIMER_DEF(heartbeatTimer);

void heartbeat_timer_initialize() {
  create_repeated_timer(&heartbeatTimer);
  start_timer(&heartbeatTimer, MS_RATE_TO_BROADCAST_HEARTBEAT, send_heartbeat_message);
}

void log_heartbeat_info(BleMessageHeartbeatReq *request) {
  char deviceId[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->deviceId, deviceId);
  char parentDeviceId[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->centralConnectionDeviceId, parentDeviceId);

  if (request->centralConnectionDeviceId == 0) {
    memcpy(parentDeviceId, "ROOT", 5);
  }

  NRF_LOG_PRINTF("HEARTBEAT: {\"id\": \"%s\", \"rawId\": %u, \"parentId\": \"%s\", \"version\": \"%u.%u\"}\r\n",
      deviceId, request->deviceId, parentDeviceId, request->majorVersion, request->minorVersion);
}

void send_heartbeat_message(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  BleMessageHeartbeatReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Heartbeat;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = get_raw_device_id();
  request.centralConnectionDeviceId = get_central_connection_device_id();

  log_heartbeat_info(&request);
  send_to_central_connection(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(request));
}

MessagePropagationType receive_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  log_heartbeat_info((BleMessageHeartbeatReq *)dataPacket);
  return PropagateToCentral;
}

