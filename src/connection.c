#include "connection.h"
#include "gap.h"

static inline uint32_t get_device_id() {
  // define out direct device memory access when testing
  #ifdef TESTING
    return 0xDEADBEEF; // arbitrary device id for tests
  #else
    return NRF_FICR->DEVICEID[1];
  #endif
}

void connections_initialize() {
  deviceId = get_device_id();
  nodeName = malloc(NODE_NAME_SIZE);
  set_node_name_from_device_id(deviceId, nodeName);
  activeConnections = calloc(1, sizeof(connections));

  NRF_LOG_PRINTF("NodeName: %s, DeviceId: %u\r\n", nodeName, deviceId);
}


void set_connection(connection *localConnection, uint16_t connectionHandle, ble_gap_addr_t deviceAddress, ConnectionType type) {
  memcpy(&localConnection->address, &deviceAddress, sizeof(deviceAddress));
  memcpy(&localConnection->handle, &connectionHandle, sizeof(connectionHandle));
  localConnection->type = type;
  localConnection->active = true;
}


connection* set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress) {
  if (activeConnections->central.active){
    // Need better error handling for this...
    NRF_LOG_PRINTF("CONNECTION: Attempt to add central connection, but no slots are free!\r\n");
    return NULL;

  } else {
    set_connection(&activeConnections->central, connectionHandle, deviceAddress, CENTRAL);
    return &activeConnections->central;
  }
}


connection* set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress) {
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (!activeConnections->peripheral[i].active){
      set_connection(&activeConnections->peripheral[i], connectionHandle, deviceAddress, PERIPHERAL);
      return &activeConnections->peripheral[i];
    }
  }

  // Need better error handling for this...
  NRF_LOG_PRINTF("CONNECTION: Attempt to add peripheral connection, but no slots are free!\r\n");
  return NULL;
}


connection* find_active_connection_by_handle(uint16_t connectionHandle) {
  connection *central = &activeConnections->central;
  if (central->active && central->handle == connectionHandle){
    return central;
  }
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    connection *peripheral = &activeConnections->peripheral[i];
    if (peripheral->active && peripheral->handle == connectionHandle){
      return peripheral;
    }
  }
  return 0;
}


ConnectionType unset_connection(uint16_t connectionHandle) {
  connection *lostConnection = find_active_connection_by_handle(connectionHandle);
  if (lostConnection != 0){
    ConnectionType lostConnectionType = lostConnection->type;
    memset(lostConnection, 0, sizeof(connection));
    return lostConnectionType;
  } else {
    return INVALID;
  }
}


MessagePropagationType update_connection_info(uint16_t connectionHandle, uint8_t *dataPacket) {
  BleMessageConnectionInfoReq *msg = (BleMessageConnectionInfoReq *)dataPacket;
  connection *conn = find_active_connection_by_handle(connectionHandle);
  if (conn != 0) {
    conn->majorVersion = msg->majorVersion;
    conn->minorVersion = msg->minorVersion;
    conn->deviceId = msg->deviceId;
  }
  return DoNotPropagate;
}


char* get_connection_type_name(connection *conn) {
  switch (conn->type)
  {
    case CENTRAL:
      return "CENTRAL";
    case PERIPHERAL:
      return "PERIPHERAL";
    default:
      return "INVALID";
  }
}


char* get_connection_info(connection *conn, char* result) {
  if (conn->active){
    char handle[6];
    sprintf(handle, "%u", conn->handle);

    strcpy(result, "\n\r   [");
    strcat(result, get_connection_type_name(conn));
    strcat(result, "] [");
    strcat(result, handle);
    strcat(result, "] ");
    if (conn->deviceId != 0) {
      char version[9];
      sprintf(version, "v%u.%u", conn->majorVersion, conn->minorVersion);

      char *nodeName = malloc(NODE_NAME_SIZE);
      set_node_name_from_device_id(conn->deviceId, nodeName);
      strcat(result, nodeName);
      strcat(result, ", ");
      strcat(result, version);
      free(nodeName);
    } else {
      uint8_t *addr = conn->address.addr;
      for (int i = 0; i < 6; i++){
        char address[4];
        sprintf(address, "%u", (uint8_t)(addr[i]));

        strcat(result, address);
        strcat(result, " ");
      }
    }
    strcat(result, "\0");
  } else {
    strcpy(result, " ");
  }
  return result;
}


void print_all_connections(char** commandArray) {
  if (!central_connection_active() && !peripheral_connections_active()) {
    NRF_LOG_PRINTF("Connection details: DISCONNECTED\r\n");
    return;
  }

  char centralInfo[50], peripheral1Info[50], peripheral2Info[50], peripheral3Info[50];
  NRF_LOG_PRINTF("Connection details: %s%s%s%s\r\n",
      get_connection_info(&activeConnections->central, centralInfo),
      get_connection_info(&activeConnections->peripheral[0], peripheral1Info),
      get_connection_info(&activeConnections->peripheral[1], peripheral2Info),
      get_connection_info(&activeConnections->peripheral[2], peripheral3Info));
}


bool central_connection_active() {
  return activeConnections->central.active;
}


bool peripheral_connections_active() {
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (activeConnections->peripheral[i].active) return true;
  }
  return false;
}


bool all_peripheral_connections_active() {
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (!(activeConnections->peripheral[i].active)) return false;
  }
  return true;
}


void share_connection_info(connection *targetConnection) {
  BleMessageConnectionInfoReq request;
  memset(&request, 0, sizeof(request));
  request.head.messageType = ConnectionInfo;
  request.majorVersion = APP_VERSION_MAIN;
  request.minorVersion = APP_VERSION_SUB;
  request.deviceId = deviceId;

  send_to_single_connection(targetConnection, (uint8_t *)&request, sizeof(request));
}

