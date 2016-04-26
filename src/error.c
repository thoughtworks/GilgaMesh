#include <error.h>

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
  if(error_code != NRF_SUCCESS){
    const char* error_string = getNrfErrorString(error_code);
    log("ERROR: ERROR CODE %d: %s, in file %s@%d", error_code, error_string, p_file_name, line_num);
  }
}

void HardFault_Handler(void)
{
  log("ERROR: Hard Fault");
  for (;;){
    // Endless debugger loop
  }
}
