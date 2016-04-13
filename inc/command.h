#pragma once

#include <stdint.h>

void print_help();
uint32_t broadcast_message(char* message);

uint32_t command_execute(char** parsedCommandArray, uint8_t numberOfItems);
void command_print(char** parsedCommandArray, uint8_t numberOfItems);
