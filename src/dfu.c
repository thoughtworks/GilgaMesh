#include <stdlib.h>
#include <nrf_soc.h>
#include <dfu.h>
#include <bootloader_types.h>
#include <boards.h>

void enter_dfu() {
#ifdef DFU_DEBUG
    for (size_t i = LED_START; i <= LED_STOP; i++)
    {
        nrf_gpio_cfg_output(i);
        nrf_gpio_pin_clear(i);
    }

    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = LED_START; j <= LED_STOP; j++)
        {
            nrf_gpio_pin_set(j);
            nrf_delay_ms(100);
            nrf_gpio_pin_clear(j);
        }
    }
#endif // DFU_DEBUG

    sd_power_gpregret_clr(0xFF);
    sd_power_gpregret_set(BOOTLOADER_DFU_START);
    NVIC_SystemReset();
}