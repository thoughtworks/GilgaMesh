// When mocking nvic calls, you need to #ifdef TESTING the #include line
// in your source file to include this file instead of including nrf_nvic.h

#include "cmocka_includes.h"

uint32_t sd_nvic_EnableIRQ(IRQn_Type IRQn)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_DisableIRQ(IRQn_Type IRQn)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_GetPendingIRQ(IRQn_Type IRQn, uint32_t * p_pending_irq)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_SetPendingIRQ(IRQn_Type IRQn)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_ClearPendingIRQ(IRQn_Type IRQn)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_GetPriority(IRQn_Type IRQn, uint32_t * p_priority)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_SystemReset(void)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_critical_region_enter(uint8_t * p_is_nested_critical_region)
{
  return mock_type(uint32_t);
}

uint32_t sd_nvic_critical_region_exit(uint8_t is_nested_critical_region)
{
  return mock_type(uint32_t);
}

