#include <common.h>
#include <system/timer.h>
#include <boards.h>
#include <system/gpio.h>
#include "app/blinky_app.h"

#define MS_RATE_TO_UPDATE_LED 200

SYS_TIMER_DEF(BLINKY_TIMER);
const static LED_TO_BLINK = BSP_LED_0;
static bool LED_IS_ON = false;

static void blinky_update_handler() {
    LED_IS_ON = !LED_IS_ON;
    LED_IS_ON ? sys_gpio_pin_set(LED_TO_BLINK) : sys_gpio_pin_clear(LED_TO_BLINK);
}

bool blinky_initialize() {
    sys_gpio_cfg_output(LED_TO_BLINK);
    sys_gpio_pin_clear(LED_TO_BLINK);

    if (!execute_succeeds(create_repeated_timer(&BLINKY_TIMER))) return false;
    if (!execute_succeeds(start_timer(&BLINKY_TIMER,
                                      MS_RATE_TO_UPDATE_LED,
                                      blinky_update_handler))) return false;

    return true;
}