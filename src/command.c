#include <command.h>
#include <gap.h>

void print_help() {
  simple_uart_putstring((const uint8_t *) "bro <msg>                               Broadcast a message\r\n");
  simple_uart_putstring((const uint8_t *) "cons                                    Print all connections\r\n");
  simple_uart_putstring((const uint8_t *) "adv                                     Check advertising status\r\n");
  simple_uart_putstring((const uint8_t *) "scan                                    Check scanning status\r\n");
  simple_uart_putstring((const uint8_t *) "vack <nodeId> <voterId> <hitCount>      Acknowledge vote stored\r\n");
  simple_uart_putstring((const uint8_t *) "help                                    Print this help text\r\n");
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
  else if (strcmp(parsedCommandArray[0], "adv") == 0) {
    check_advertising_status();
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "scan") == 0) {
    check_scanning_status();
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "vack") == 0) {
    log("received vote acknowledgement!");
    broadcast_vote_acknowledgement(parsedCommandArray[1], parsedCommandArray[2], parsedCommandArray[3]);
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

