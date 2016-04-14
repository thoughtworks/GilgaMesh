#include <command.h>

void print_help() {
  simple_uart_putstring((const uint8_t *) "bro <msg>    Broadcast a message\r\n");
  simple_uart_putstring((const uint8_t *) "cons         Print all connections\r\n");
  simple_uart_putstring((const uint8_t *) "help         Print this help text\r\n");
  simple_uart_putstring((const uint8_t *) "\r\n");
}

uint32_t command_execute(char **parsedCommandArray, uint8_t numberOfItems) {
  //command_print(parsedCommandArray, numberOfItems);

  if (strcmp(parsedCommandArray[0], "bro") == 0) {
    broadcast_message(parsedCommandArray[1]);
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "cons") == 0) {
    print_all_connections();
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "help") == 0) {
    print_help();
    return COMMAND_SUCCESS;
  }
  else // default:
  {
    print_help();
    return COMMAND_SUCCESS;
  }
}

