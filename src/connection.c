#include <connection.h>


connections *activeConnections;


void connections_initialize()
{
  activeConnections = calloc(1, sizeof(connections));
}


void set_central_connection(ble_gap_addr_t deviceAddress)
{
  if (activeConnections->central.exists){
    // Need better error handling for this...
    log("CONNECTION: Attempt to add central connection, but no slots are free!");

  } else {
    log("CONNECTION: I am connected to a CENTRAL device.");
    set_connection(activeConnections->central, deviceAddress);

    //If the green LED is on, we are connected as a peripheral device
    led_green_on();
  }
}


void set_peripheral_connection(ble_gap_addr_t deviceAddress)
{
  bool addedConnection = false;

  for (int i = 0; i < ATTR_MAX_PERIPHERAL_CONNS; i++){
    if (!activeConnections->peripheral[i].exists){
      log("CONNECTION: I am connected to a PERIPHERAL device (slot #%d).", (i + 1));
      set_connection(activeConnections->peripheral[i], deviceAddress);
      addedConnection = true;

      //If the red LED is on, we are connected as a central device
      led_red_on();

      break;
    }
  }

  if (!addedConnection){
    // Need better error handling for this...
    log("CONNECTION: Attempt to add peripheral connection, but no slots are free!");
  }
}


void set_connection(connection localConnection, ble_gap_addr_t deviceAddress)
{
  memcpy(&localConnection.address, &deviceAddress, sizeof(deviceAddress));
  localConnection.exists = true;

  uint8_t *addr = &localConnection.address.addr;
  log("CONNECTION: Peer address is: %u %u %u %u %u %u", *addr, *addr+1, *addr+2, *addr+3, *addr+4, *addr+5);
}
