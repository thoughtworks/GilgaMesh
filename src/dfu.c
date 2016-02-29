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

#define DEVICE_NAME_TEMPLATE                 "VB-0000"

const char *dfu_device_name_with_id() {
    static char hex_chars[] = "0123456789ABCDEF";
    static char device_name[sizeof(DEVICE_NAME_TEMPLATE)] = { 0 };
    if (device_name[0] == 0) {
        strcpy(device_name, DEVICE_NAME_TEMPLATE);
        uint32_t device_id = NRF_FICR->DEVICEID[1];
        size_t name_length = strlen(device_name);
        for (size_t i = 1; i <= 4; i++)
        {
            device_name[name_length - i] = hex_chars[device_id % 16];
            device_id /= 16;
        }
    }
    return device_name;
}