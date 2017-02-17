#include <system/gpio.h>
#include <nrf_gpio.h>

void sys_gpio_pin_clear(uint32_t pin_number) {
    nrf_gpio_pin_clear(pin_number);
}

void sys_gpio_pin_set(uint32_t pin_number) {
    nrf_gpio_pin_set(pin_number);
}

void sys_gpio_cfg_output(uint32_t pin_number) {
    nrf_gpio_cfg_output(pin_number);
}