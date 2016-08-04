#include <nrf_nvic.h>

uint32_t sys_ClearPendingIRQ(IRQn_Type irqN) {
  return sd_nvic_ClearPendingIRQ(irqN);
}