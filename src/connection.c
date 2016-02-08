#include <connection.h>


void connections_initialize()
{
  activeConnections = calloc(1, sizeof(connections));
}


void set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress)
{
  if (activeConnections->central.active){
    // Need better error handling for this...
    log("CONNECTION: Attempt to add central connection, but no slots are free!");

  } else {
    log("CONNECTION: I am connected to a CENTRAL device.");
    set_connection(&activeConnections->central, connectionHandle, deviceAddress, CENTRAL);
  }
}


void set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress)
{
  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (!activeConnections->peripheral[i].active){
      log("CONNECTION: I am connected to a PERIPHERAL device (slot #%d).", (i + 1));
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
  return NULL;
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
  return NULL;
}


ConnectionType unset_connection(uint16_t connectionHandle)
{
  connection *lostConnection = find_active_connection(connectionHandle);
  if (lostConnection != NULL){
    log("CONNECTION: Disconnected from device with connection handle %u", connectionHandle);

    ConnectionType lostConnectionType = lostConnection->type;
    memset(lostConnection, 0, sizeof(connection));
    handle_connection_change();

    return lostConnectionType;
  } else {
    log("We didn't find a matching connection! That SUCKS.");
    return INVALID;
  }
}


void print_connection_status(connection *conn, uint8_t* name)
{
  log("[%s] This connection is active: %u", name, conn->active);
  log("[%s] Event connection handle is %u", name, conn->connectionHandle);
  uint8_t *addr = &conn->address.addr;
  log("[%s] Peer address is: %u %u %u %u %u %u", name, *addr, *addr+1, *addr+2, *addr+3, *addr+4, *addr+5);
}


void print_all_connections()
{
  print_connection_status(&activeConnections->central, "CENTRAL");
  print_connection_status(&activeConnections->peripheral[0], "PERIPHERAL #1");
  print_connection_status(&activeConnections->peripheral[1], "PERIPHERAL #2");
  print_connection_status(&activeConnections->peripheral[2], "PERIPHERAL #3");
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
