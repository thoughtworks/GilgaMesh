#include <connection.h>

#include "cmocka_includes.h"

void connections_initialize(void){

}

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

void print_all_connections(void){
   char centralInfo[50], peripheral1Info[50], peripheral2Info[50], peripheral3Info[50];
   log("Connection details have changed:\n\r   Family ID: %u %s%s%s%s",
       familyId,
       get_connection_info(&activeConnections->central, centralInfo),
       get_connection_info(&activeConnections->peripheral[0], peripheral1Info),
       get_connection_info(&activeConnections->peripheral[1], peripheral2Info),
       get_connection_info(&activeConnections->peripheral[2], peripheral3Info));
}

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

char* get_connection_info(connection *conn, char* result) {
   return mock_type(char*);
}