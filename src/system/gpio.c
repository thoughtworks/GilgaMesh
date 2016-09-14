#include <system/gpio.h>
#include <nrf_gpio.h>

void sys_gpio_pin_clear(uint32_t pin_number) {
    nrf_gpio_pin_clear(pin_number);
}