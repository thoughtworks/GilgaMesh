#include "command.h"
#include "cmocka_includes.h"

bool command_initialize(void) {
  return true;
}

void command_execute(char** parsedCommandArray, uint8_t numberOfItems) { }
void mesh_add_terminal_command(char* commandName, char* commandDescription, mesh_command_handler callback) {
  check_expected(commandName);
}
