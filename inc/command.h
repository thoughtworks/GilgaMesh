#pragma once

#include <stdint.h>

typedef void (*mesh_command_handler)(char **parsedCommandArray);

void command_initialize(void);
void command_execute(char** parsedCommandArray, uint8_t numberOfItems);
void mesh_add_terminal_command(char* commandName, char* commandDescription, mesh_command_handler callback);
