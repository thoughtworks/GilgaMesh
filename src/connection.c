#include <ble_conn_state.h>
#include "connection.h"
#include "device.h"
#include "gap.h"


static connection (*activeConnections)[MAX_TOTAL_CONNECTIONS];

void connections_initialize() {
  activeConnections = malloc(sizeof(*activeConnections));
  memset(activeConnections, 0, sizeof(*activeConnections));

  char short_hex_device_id[HEX_DEVICE_ID_LENGTH];
  get_short_hex_device_id(get_raw_device_id(), short_hex_device_id);
  NRF_LOG_PRINTF("DeviceId: pretty: %s, raw: %u\r\n", short_hex_device_id, get_raw_device_id());
}


connection* set_connection(uint16_t connectionHandle, ConnectionType connectionType) {
  for (int i = 0; i < MAX_TOTAL_CONNECTIONS; i++){
    connection *conn = &(*activeConnections)[i];
    if (!conn->active){
      conn->handle = connectionHandle;
      conn->type = connectionType;
      conn->active = true;
      return conn;
    }
  }
  return NULL;
}


connection* find_active_connection_by_handle(uint16_t connectionHandle) {
  for (int i = 0; i < MAX_TOTAL_CONNECTIONS; i++){
    connection *connection = &(*activeConnections)[i];
    if (connection->active && connection->handle == connectionHandle){
      return connection;
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
  if (!is_connection_active(connectionHandle)) return;

  char result[50];
  char handle[6];
  sprintf(handle, "%u", connectionHandle);

  strcpy(result, "   [");
  strcat(result, get_connection_type_name(connectionHandle));
  strcat(result, "] [");
  strcat(result, handle);
  strcat(result, "] ");
  connection *conn = find_active_connection_by_handle(connectionHandle);
  if (conn->deviceId != 0) {
    char version[9];
    sprintf(version, "v%u.%u", conn->majorVersion, conn->minorVersion);

    char short_hex_device_id[HEX_DEVICE_ID_LENGTH];
    strcat(result, get_short_hex_device_id(conn->deviceId, short_hex_device_id));
    strcat(result, ", ");
    strcat(result, version);
  } else {
    strcat(result, "Unknown Device");
  }
  strcat(result, "\0");

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

bool is_connection_active(uint16_t connectionHandle) {
  return ble_conn_state_status(connectionHandle) == BLE_CONN_STATUS_CONNECTED;
}

uint32_t get_central_connection_device_id() {
  for(int i = 0; i < MAX_TOTAL_CONNECTIONS; i++) {
    connection *conn = &(*activeConnections)[i];
    if(conn->type == CENTRAL) {
      return conn->deviceId;
    }
  }
  return 0;
}

void print_all_connections(char** commandArray, uint8_t numCommands) {
  UNUSED_PARAMETER(commandArray);
  UNUSED_PARAMETER(numCommands);

  NRF_LOG_PRINTF("Connection details: %s\r\n", is_connected() ? "" : "DISCONNECTED");
  sdk_mapped_flags_key_list_t connKeyList = ble_conn_state_conn_handles();
  for (int i = 0; i < connKeyList.len; i++) {
    print_single_connection_info(connKeyList.flag_keys[i]);
  }
}
