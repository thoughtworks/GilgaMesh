#include "cmocka_includes.h"

uint32_t broadcast_message(char* message) {

}

uint32_t command_execute(char** parsedCommandArray, uint8_t numberOfItems) {
   check_expected(numberOfItems);

   return mock_type(uint32_t);
}

void command_print(char** parsedCommandArray, uint8_t numberOfArgs) {
   return mock_type(uint32_t);
}

