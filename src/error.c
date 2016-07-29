#include "error.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
  UNUSED_PARAMETER(id);
  UNUSED_PARAMETER(pc);

  error_info_t *error_info = (error_info_t *) info;
  if(error_info->err_code != NRF_SUCCESS){
    NRF_LOG_PRINTF("ERROR: ERROR CODE %d, in file %s@%d\r\n",
                   error_info->err_code, error_info->p_file_name, error_info->line_num);
  }
}