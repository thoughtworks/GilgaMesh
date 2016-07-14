#include <command.h>
#include <gap.h>
#include <timer.h>
#include "terminal.h"

void print_help() {
  terminal_putstring("bro <msg>                             Broadcast a message\r\n");
  terminal_putstring("cons                                  Print all connections\r\n");
  terminal_putstring("adv                                   Check advertising status\r\n");
  terminal_putstring("scan                                  Check scanning status\r\n");
  terminal_putstring("vack <nodeId> <voterId> <timestamp>   Acknowledge vote stored\r\n");
  terminal_putstring("grp <nodeId> <group>                  Set the group for a node\r\n");
  terminal_putstring("val <nodeId> <value>                  Set the value for a node\r\n");
  terminal_putstring("rtc                                   Real time clock commands\r\n");
  terminal_putstring("hb <on/off>                           Heartbeat on/off\r\n");
  terminal_putstring("time                                  Print date and time\r\n");
  terminal_putstring("pd                                    Power down NFC\r\n");
  terminal_putstring("help                                  Print this help text\r\n");
  terminal_putstring("\r\n");
}

void print_help_rtc() {
  terminal_putstring("rtc info                              Show RTC HW information\r\n");
  terminal_putstring("rtc set <field> <value>               Set RTC\r\n");
  terminal_putstring("\r\n");
  terminal_putstring("    <field> = [ year(0-99), month(1-12), weekday(0-6), day(1-31)\r\n");
  terminal_putstring("                hour(0-23), minute(0-59) or second(0-59) ]\r\n");
  terminal_putstring("\r\n");
}

uint32_t command_execute(char **parsedCommandArray, uint8_t numberOfItems) {
  if (strcmp(parsedCommandArray[0], "bro") == 0) {
    broadcast_logging_message(parsedCommandArray[1]);
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
    broadcast_vote_acknowledgement(parsedCommandArray[1], parsedCommandArray[2], parsedCommandArray[3]);
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "grp") == 0) {
    broadcast_group_value_update(parsedCommandArray[1], parsedCommandArray[2], true);
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "val") == 0) {
    broadcast_group_value_update(parsedCommandArray[1], parsedCommandArray[2], false);
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "help") == 0) {
    print_help();
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "time") == 0) {
    rtc_print_date_and_time();
    return COMMAND_SUCCESS;
  }
  else if (strcmp(parsedCommandArray[0], "rtc") == 0) {
    if (strcmp(parsedCommandArray[1], "info") == 0) {
      rtc_print_status();
      return COMMAND_SUCCESS;
    }
    else if (strcmp(parsedCommandArray[1], "set") == 0) {
      rtc_set_field(parsedCommandArray[2], parsedCommandArray[3]);
    }
    else {
      print_help_rtc();
      return COMMAND_SUCCESS;
    }
  }
  else if (strcmp(parsedCommandArray[0], "hb") == 0) {
    if (strcmp(parsedCommandArray[1], "off") == 0) {
      turn_off_heartbeats();
      return COMMAND_SUCCESS;
    }
    else if (strcmp(parsedCommandArray[1], "on") == 0) {
      turn_on_heartbeats();
      return COMMAND_SUCCESS;
    }
    else {
      print_help();
      return COMMAND_SUCCESS;
    }
  }
  else if (strcmp(parsedCommandArray[0], "pd") == 0) {
#ifdef NFC_DEBUG
    power_down();
#else
    terminal_putstring("Cannot power down; NFC not enabled\r\n");
#endif
    return COMMAND_SUCCESS;
  }
  else // default:
  {
    print_help();
    return COMMAND_SUCCESS;
  }
}

