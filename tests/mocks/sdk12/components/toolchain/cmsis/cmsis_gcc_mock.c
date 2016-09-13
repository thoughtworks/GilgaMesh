#include <cmsis_gcc.h>
#include "cmocka_includes.h"

void __enable_irq(void) {}

void __disable_irq(void) {}

uint32_t __get_CONTROL(void) {
    return mock_type(uint32_t);
}

void __set_CONTROL(uint32_t control) {}

uint32_t __get_IPSR(void) {
    return mock_type(uint32_t);
}

uint32_t __get_APSR(void){
    return mock_type(uint32_t);
}

uint32_t __get_xPSR(void) {
    return mock_type(uint32_t);
}

uint32_t __get_PSP(void) {
    return mock_type(uint32_t);
}

void __set_PSP(uint32_t topOfProcStack) {}

uint32_t __get_MSP(void) {
    return mock_type(uint32_t);
}

void __set_MSP(uint32_t topOfMainStack) {}

uint32_t __get_PRIMASK(void) {
    return mock_type(uint32_t);
}

void __set_PRIMASK(uint32_t priMask) {}

void __NOP(void) {}

void __WFI(void) {}

void __WFE(void) {}

void __SEV(void) {}

void __ISB(void) {}

void __DSB(void) {}

void __DMB(void) {}

uint32_t __REV(uint32_t value) {
    return mock_type(uint32_t);
}

uint32_t __REV16(uint32_t value) {
    return mock_type(uint32_t);
}

int32_t __REVSH(int32_t value) {
    return mock_type(int32_t);
}

uint32_t __ROR(uint32_t op1, uint32_t op2) {
    return mock_type(uint32_t);
}

uint32_t __RBIT(uint32_t value) {
    return mock_type(uint32_t);
}
