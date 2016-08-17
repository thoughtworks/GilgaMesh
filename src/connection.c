#include <ble_conn_state.h>
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
  for (int i = 0; i < MAX_PERIPHERAL_CONNECTIONS; i++){
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
  for (int i = 0; i < MAX_PERIPHERAL_CONNECTIONS; i++){
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


char* get_connection_type_name(uint16_t connHandle) {
  switch (ble_conn_state_role(connHandle))
  {
    case BLE_GAP_ROLE_CENTRAL:
      return "CENTRAL";
    case BLE_GAP_ROLE_PERIPH:
      return "PERIPHERAL";
    default:
      return "INVALID";
  }
}


void print_single_connection_info(uint16_t connectionHandle) {
  char result[50];
  if (ble_conn_state_status(connectionHandle) == BLE_CONN_STATUS_CONNECTED){
    char handle[6];
    sprintf(handle, "%u", connectionHandle);

    strcpy(result, "   [");
    strcat(result, get_connection_type_name(connectionHandle));
    strcat(result, "] [");
    strcat(result, handle);
    strcat(result, "] ");
    connection* conn = find_active_connection_by_handle(connectionHandle);
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

  NRF_LOG_PRINTF("%s\r\n", result);
}

bool is_connected() {
  return ble_conn_state_n_connections() > 0;
}

bool central_connection_active() {
  return ble_conn_state_n_peripherals() > 0;
}

bool all_peripheral_connections_active() {
  return ble_conn_state_n_centrals() >= MAX_PERIPHERAL_CONNECTIONS;
}

void print_all_connections(char** commandArray) {
  UNUSED_PARAMETER(commandArray);

  if (!is_connected()) {
    NRF_LOG_PRINTF("Connection details: DISCONNECTED\r\n");
    return;
  }

  NRF_LOG_PRINTF("Connection details: \r\n");
  sdk_mapped_flags_key_list_t connKeyList = ble_conn_state_conn_handles();
  for (int i = 0; i < connKeyList.len; i++) {
    if (ble_conn_state_status(connKeyList.flag_keys[i]) == BLE_CONN_STATUS_CONNECTED) {
      print_single_connection_info(connKeyList.flag_keys[i]);
    }
  }
}
