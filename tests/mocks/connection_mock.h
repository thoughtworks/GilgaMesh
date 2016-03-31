#pragma once

void connections_initialize(void){}

void set_central_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress){
   check_expected(connectionHandle);
}

void set_peripheral_connection(uint16_t connectionHandle, ble_gap_addr_t deviceAddress){
   check_expected(connectionHandle);
}

ConnectionType unset_connection(uint16_t connectionHandle){
   check_expected(connectionHandle);
   return mock_type(ConnectionType);
}

void print_all_connections(void){}

connection* find_active_connection_by_address(ble_gap_addr_t address){
   return mock_ptr_type(connection*);
}

bool central_connection_active(void){
   return mock_type(bool);
}

bool peripheral_connections_active(void){
   return mock_type(bool);
}

uint16_t* get_active_connection_handles(uint16_t *handles, uint8_t *connectionCount) {
   return mock_type(uint16_t*);
}