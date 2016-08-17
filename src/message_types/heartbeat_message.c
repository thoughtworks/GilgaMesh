#include "message_types/heartbeat_message.h"
#include <stdlib.h>
#include "conversion.h"
#include "gatt.h"
#include "version.h"


void log_heartbeat_info(BleMessageHeartbeatReq *request) {
  char *nodeName = malloc(NODE_NAME_SIZE);
  char *parentNodeName = malloc(NODE_NAME_SIZE);

  set_node_name_from_device_id(request->deviceId, nodeName);
  if (request->centralConnectionDeviceId == 0) {
    strncpy(parentNodeName, "ROOT", 5);
  } else {
    set_node_name_from_device_id(request->centralConnectionDeviceId, parentNodeName);
  }

  NRF_LOG_PRINTF("HEARTBEAT: {\"id\": \"%s\", \"rawId\": %u, \"parentId\": \"%s\", \"version\": \"%u.%u\"}\r\n",
      nodeName, request->deviceId, parentNodeName, request->majorVersion, request->minorVersion);

  free(nodeName);
  free(parentNodeName);
}


void send_heartbeat_message(void *data, uint16_t dataLength) {
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(dataLength);

  BleMessageHeartbeatReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = Heartbeat;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = deviceId;
  request.centralConnectionDeviceId = get_central_connection_device_id();

  log_heartbeat_info(&request);
  send_to_central_connection(BLE_CONN_HANDLE_INVALID, (uint8_t *) &request, sizeof(request));
}

MessagePropagationType receive_heartbeat_message(uint16_t connectionHandle, uint8_t *dataPacket) {
  UNUSED_PARAMETER(connectionHandle);
  log_heartbeat_info((BleMessageHeartbeatReq *)dataPacket);
  return PropagateToCentral;
}


