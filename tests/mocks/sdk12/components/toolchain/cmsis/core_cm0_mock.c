#include <core_cm0.h>
#include <nrf51.h>
#include "cmocka_includes.h"

void NVIC_EnableIRQ(IRQn_Type IRQn) {}

void NVIC_DisableIRQ(IRQn_Type IRQn)  {}

uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn) {
    return mock_type(uint32_t);
}

void NVIC_SetPendingIRQ(IRQn_Type IRQn) {}

void NVIC_ClearPendingIRQ(IRQn_Type IRQn) {}

void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority) {}

uint32_t NVIC_GetPriority(IRQn_Type IRQn) {
    return mock_type(uint32_t);
}

void NVIC_SystemReset(void) {}