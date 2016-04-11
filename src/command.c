#include <string.h>
#include "simple_uart.h"
#include "conversion.h"
#include "connection.h"
#include "gatt.h"

#define COMMAND_ERROR 1
#define COMMAND_SUCCESS 0

void print_help() {
   simple_uart_putstring("bro <msg>    Broadcast a message\r\n");
   simple_uart_putstring("cons         Print all connections\r\n");
   simple_uart_putstring("help         Print this help text\r\n");
   simple_uart_putstring("\r\n");
}

uint32_t broadcast_message(char* message) {
   uint16_t connectionHandles[ATTR_MAX_CONNECTIONS] = { 0 };
   uint8_t connectionCount = 0;
   get_active_connection_handles(&connectionHandles, &connectionCount);

   if (connectionCount == 0) {
      simple_uart_putstring("ERROR: No active connections\r\n");
      return COMMAND_ERROR;
   }

   for (int i = 0 ; i < connectionCount ; i++) {
      send_broadcast_message(connectionHandles[i], message);
   }

   return COMMAND_SUCCESS;
}

uint32_t command_execute(char** parsedCommandArray, uint8_t numberOfItems) {
   //command_print(parsedCommandArray, numberOfItems);

   if (strcmp(parsedCommandArray[0], "bro") == 0)
   {
      return broadcast_message(parsedCommandArray[1]);
   }
   else if (strcmp(parsedCommandArray[0], "cons") == 0)
   {
      print_all_connections();
      return COMMAND_SUCCESS;
   }
   else if (strcmp(parsedCommandArray[0], "help") == 0)
   {
      print_help();
      return COMMAND_SUCCESS;
   }
   else // default:
   {
      print_help();
      return COMMAND_SUCCESS;
   }
}

void command_print(char** parsedCommandArray, uint8_t numberOfItems) {
   simple_uart_putstring("\r\nCOMMAND: ");
   simple_uart_putstring(parsedCommandArray[0]);
   simple_uart_putstring("\r\n");

   for (int i = 1; i < numberOfItems; ++i) {
      simple_uart_putstring("ARG");
      simple_uart_putstring(int_to_string(i));
      simple_uart_putstring(": ");
      simple_uart_putstring(parsedCommandArray[i]);
      simple_uart_putstring("\r\n");
   }
}

