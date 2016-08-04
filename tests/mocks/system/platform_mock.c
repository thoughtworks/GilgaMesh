#include <system/platform.h>
#include "cmocka_includes.h"

uint32_t sys_ClearPendingIRQ(IRQn_Type irqN) {
  check_expected(irqN);
  return mock_type(uint32_t);
}