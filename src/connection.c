#include <connection.h>

void connections_initialize()
{
  nodeId = (uint16_t) NRF_FICR->DEVICEID[1] % 15000 + 1;
  familyId = nodeId << 16;
  activeConnections = calloc(1, sizeof(connections));

  log("NodeId: %d, familyId: %u", nodeId, familyId);
}


void set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress)
{
  if (activeConnections->central.active){
    // Need better error handling for this...
    log("CONNECTION: Attempt to add central connection, but no slots are free!");

  } else {
    set_connection(&activeConnections->central, connectionHandle, deviceAddress, CENTRAL);
  }
}


void set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress)
{
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (!activeConnections->peripheral[i].active){
      set_connection(&activeConnections->peripheral[i], connectionHandle, deviceAddress, PERIPHERAL);
      return;
    }
  }

  // Need better error handling for this...
  log("CONNECTION: Attempt to add peripheral connection, but no slots are free!");
}


void set_connection(connection *localConnection, uint16_t connectionHandle, ble_gap_addr_t deviceAddress, ConnectionType type)
{
  memcpy(&localConnection->address, &deviceAddress, sizeof(deviceAddress));
  memcpy(&localConnection->connectionHandle, &connectionHandle, sizeof(connectionHandle));
  localConnection->type = type;
  localConnection->active = true;

  handle_connection_change();
}


connection* find_active_connection(uint16_t connectionHandle)
{
  connection *central = &activeConnections->central;
  if (central->active && central->connectionHandle == connectionHandle){
    return central;
  }
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    connection *peripheral = &activeConnections->peripheral[i];
    if (peripheral->active && peripheral->connectionHandle == connectionHandle){
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
  connection *lostConnection = find_active_connection(connectionHandle);
  if (lostConnection != 0){
    ConnectionType lostConnectionType = lostConnection->type;
    memset(lostConnection, 0, sizeof(connection));
    handle_connection_change();

    return lostConnectionType;
  } else {
    return INVALID;
  }
}


#define INT_DIGITS 19/* enough for 64 bit integer */

char *itoa(uint8_t i)
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}


char* get_connection_info(connection *conn, uint8_t* name, char* connectionInfo)
{
  if (conn->active){
    uint8_t *addr = &conn->address.addr;
    strcpy(connectionInfo, "\n\r   [");
    strcat(connectionInfo, name);
    strcat(connectionInfo, "]");
    for (int i = 0; i < 6; i++){
      strcat(strcat(connectionInfo, " "), itoa(*addr+i));
    }
    strcat(connectionInfo, "\0");
  } else {
    strcpy(connectionInfo, " ");
  }
  return connectionInfo;
}


void print_all_connections()
{
  char centralInfo[50], peripheral1Info[50], peripheral2Info[50], peripheral3Info[50];
  log("Connection details have changed:\n\r   Family ID: %u %s%s%s%s",
      familyId,
      get_connection_info(&activeConnections->central, "CENTRAL", centralInfo),
      get_connection_info(&activeConnections->peripheral[0], "PERIPHERAL", peripheral1Info),
      get_connection_info(&activeConnections->peripheral[1], "PERIPHERAL", peripheral2Info),
      get_connection_info(&activeConnections->peripheral[2], "PERIPHERAL", peripheral3Info));
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


uint16_t* get_active_connection_handles(uint16_t *handles, uint8_t *connectionCount)
{
  *connectionCount = 0;
  if (central_connection_active()){
    handles[*connectionCount] = activeConnections->central.connectionHandle;
    (*connectionCount)++;
  }
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (activeConnections->peripheral[i].active){
      handles[*connectionCount] = activeConnections->peripheral[i].connectionHandle;
      (*connectionCount)++;
    }
  }
}

void handle_connection_change()
{
  if (central_connection_active()){
    led_green_on();
  } else {
    led_green_off();
  }

  if (peripheral_connections_active()){
    led_red_on();
  } else {
    led_red_off();
  }
}