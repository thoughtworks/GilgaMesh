#include <error.h>
#include "cmocka_includes.h"

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
   check_expected(error_code);
   check_expected(line_num);
}

void HardFault_Handler(void)
{
   for (;;){
      // Endless debugger loop
   }
}
