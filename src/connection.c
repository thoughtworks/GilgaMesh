#include <connection.h>
#include <stdlib.h>

static inline uint32_t get_device_id() {
  // define out direct device memory access when testing
  #ifdef TESTING
    return 0xDEADBEEF; // arbitrary device id for tests
  #else
    return NRF_FICR->DEVICEID[1];
  #endif
}

void connections_initialize()
{
  familyId = generate_family_id();
  deviceId = get_device_id();
  nodeName = malloc(NODE_NAME_SIZE);
  set_node_name_from_device_id(deviceId, nodeName);
  activeConnections = calloc(1, sizeof(connections));

  log("FamilyId: %u, NodeName: %s, DeviceId: %u", familyId, nodeName, deviceId);
}


uint32_t generate_family_id()
{
  uint32_t nodeId = get_device_id() % 15000 + 1;
  return nodeId << 16;
}


void set_connection(connection *localConnection, uint16_t connectionHandle, ble_gap_addr_t deviceAddress, ConnectionType type)
{
  memcpy(&localConnection->address, &deviceAddress, sizeof(deviceAddress));
  memcpy(&localConnection->handle, &connectionHandle, sizeof(connectionHandle));
  localConnection->type = type;
  localConnection->active = true;
  display_connection_status();
}


connection* set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress)
{
  if (activeConnections->central.active){
    // Need better error handling for this...
    log("CONNECTION: Attempt to add central connection, but no slots are free!");
    return NULL;

  } else {
    set_connection(&activeConnections->central, connectionHandle, deviceAddress, CENTRAL);
    return &activeConnections->central;
  }
}


connection* set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress)
{
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (!activeConnections->peripheral[i].active){
      set_connection(&activeConnections->peripheral[i], connectionHandle, deviceAddress, PERIPHERAL);
      return &activeConnections->peripheral[i];
    }
  }

  // Need better error handling for this...
  log("CONNECTION: Attempt to add peripheral connection, but no slots are free!");
  return NULL;
}


connection* find_active_connection_by_handle(uint16_t connectionHandle)
{
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


bool addresses_match(ble_gap_addr_t address1, ble_gap_addr_t address2)
{
  if (address1.addr_type != address2.addr_type) return false;
  if (memcmp(&address1.addr, &address2.addr, sizeof(address1.addr)) != 0) return false;
  return true;
}


connection* find_active_connection_by_address(ble_gap_addr_t address)
{
  connection *central = &activeConnections->central;
  if (central->active && addresses_match(central->address, address)){
    return central;
  }
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    connection *peripheral = &activeConnections->peripheral[i];
    if (peripheral->active && addresses_match(peripheral->address, address)){
      return peripheral;
    }
  }
  return 0;
}


ConnectionType unset_connection(uint16_t connectionHandle)
{
  connection *lostConnection = find_active_connection_by_handle(connectionHandle);
  if (lostConnection != 0){
    ConnectionType lostConnectionType = lostConnection->type;
    memset(lostConnection, 0, sizeof(connection));
    display_connection_status();

    return lostConnectionType;
  } else {
    return INVALID;
  }
}


void update_connection_info(uint16_t connectionHandle, BleMessageConnectionInfoReq *msg) {
  connection *conn = find_active_connection_by_handle(connectionHandle);
  if (conn != 0) {
    conn->majorVersion = msg->majorVersion;
    conn->minorVersion = msg->minorVersion;
    conn->deviceId = msg->deviceId;
  }
}


char* get_connection_type_name(connection *conn)
{
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


char* get_connection_info(connection *conn, char* result)
{
  if (conn->active){
    uint8_t *addr = conn->address.addr;
    strcpy(result, "\n\r   [");
    strcat(result, get_connection_type_name(conn));
    strcat(result, "] [");
    strcat(result, int_to_string(conn->handle));
    strcat(result, "] ");
    if (conn->deviceId != 0) {
      char *nodeName = malloc(NODE_NAME_SIZE);
      set_node_name_from_device_id(conn->deviceId, nodeName);
      strcat(result, nodeName);
      strcat(result, ", v");
      strcat(result, int_to_string(conn->majorVersion));
      strcat(result, ".");
      strcat(result, int_to_string(conn->minorVersion));
      free(nodeName);
    } else {
      for (int i = 0; i < 6; i++){
        strcat(result, int_to_string((uint16_t)(*addr+i)));
        strcat(result, " ");
      }
    }
    strcat(result, "\0");
  } else {
    strcpy(result, " ");
  }
  return result;
}


void print_all_connections()
{
  if (!central_connection_active() && !peripheral_connections_active()) {
    log("Connection details: DISCONNECTED");
    return;
  }

  char centralInfo[50], peripheral1Info[50], peripheral2Info[50], peripheral3Info[50];
  log("Connection details: %s%s%s%s",
      get_connection_info(&activeConnections->central, centralInfo),
      get_connection_info(&activeConnections->peripheral[0], peripheral1Info),
      get_connection_info(&activeConnections->peripheral[1], peripheral2Info),
      get_connection_info(&activeConnections->peripheral[2], peripheral3Info));
}


bool central_connection_active()
{
  return activeConnections->central.active;
}


bool peripheral_connections_active()
{
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (activeConnections->peripheral[i].active) return true;
  }
  return false;
}


bool all_peripheral_connections_active()
{
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (!(activeConnections->peripheral[i].active)) return false;
  }
  return true;
}


bool connected_to_mesh()
{
  return central_connection_active() || peripheral_connections_active();
}


void display_connection_status()
{
  if (connected_to_mesh()){
    led_blue_on();
    led_green_off();
  } else {
    led_green_on();
    led_blue_off();
  }
}


uint16_t* get_active_connection_handles(uint16_t *handles, uint8_t *connectionCount)
{
  *connectionCount = 0;
  if (central_connection_active()){
    handles[*connectionCount] = activeConnections->central.handle;
    (*connectionCount)++;
  }
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (activeConnections->peripheral[i].active){
      handles[*connectionCount] = activeConnections->peripheral[i].handle;
      (*connectionCount)++;
    }
  }
  return handles;
}
