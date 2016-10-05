#include "message_types/heartbeat_message.h"

#include <stdlib.h>

#include "app/uart.h"
#include "system/log.h"
#include "system/timer.h"
#include "device.h"
#include "gatt.h"

SYS_TIMER_DEF(heartbeatTimer);

static BleMessageType heartbeatMessageType;

bool heartbeat_message_initialize() {
  heartbeatMessageType = register_message_type(receive_heartbeat_message);

  if(!execute_succeeds(create_repeated_timer(&heartbeatTimer))) return false;
  if(!execute_succeeds(start_timer(&heartbeatTimer,
                                   HEARTBEAT_MESSAGE_FREQUENCY_IN_MS,
                                   send_heartbeat_message))) return false;

  return true;
}

void log_heartbeat_info(BleMessageHeartbeatReq *request) {
  char deviceId[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->deviceId, deviceId);
  char parentDeviceId[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(request->centralConnectionDeviceId, parentDeviceId);

  if (request->centralConnectionDeviceId == 0) {
    memcpy(parentDeviceId, "ROOT", 5);
  }

  char heartbeatLog[256];
  sprintf(heartbeatLog, "HEARTBEAT: {\"id\": \"%s\", \"rawId\": %u, \"parentId\": \"%s\", \"version\": \"%u.%u\"}\r\n",
          deviceId, (unsigned int) request->deviceId, parentDeviceId, request->majorVersion, request->minorVersion);


  MESH_LOG("%s", heartbeatLog);
  log_to_uart(heartbeatLog, sizeof(heartbeatLog));
}

void send_heartbeat_message() {
  BleMessageHeartbeatReq request;
  memset(&request, 0, sizeof(request));
  request.messageType = heartbeatMessageType;
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

