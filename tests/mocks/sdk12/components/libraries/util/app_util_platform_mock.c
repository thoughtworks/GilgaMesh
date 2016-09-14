#include <app_util_platform.h>
#include "cmocka_includes.h"


void app_util_disable_irq(void) {}

void app_util_enable_irq(void) {}

void app_util_critical_region_enter(uint8_t *p_nested) {}

void app_util_critical_region_exit(uint8_t nested) {}

uint8_t current_int_priority_get(void) {
    return mock_type(uint8_t);
}

uint8_t privilege_level_get(void) {
    return mock_type(uint8_t);
}
