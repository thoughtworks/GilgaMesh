#include "cmocka_includes.h"

uint32_t command_execute(char** parsedCommandArray, uint8_t numberOfItems) {
   check_expected(numberOfItems);

   return mock_type(uint32_t);
}

