#include <system/delay.h>
#include <nrf_delay.h>

void system_delay_ms(uint32_t number_of_ms) {
    nrf_delay_ms(number_of_ms);
}