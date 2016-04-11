#include "simple_uart.h"
#include "conversion.h"

uint32_t command_execute(char** parsedCommandArray, uint8_t numberOfItems) {
   command_print(parsedCommandArray, numberOfItems);
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

