#include <timer.h>

static app_timer_id_t mainTimerMsId;
uint16_t timeSinceLastHandlerCalled = 0;
uint32_t timeSinceLastBlink = 0;
uint16_t timeBetweenBlinks = 2000;

void ble_timer_dispatch(void * p_context)
{
    timeSinceLastHandlerCalled += ATTR_TIMER_TICK_MS;
    timeSinceLastBlink += ATTR_TIMER_TICK_MS;
}


void timer_initialize(void){
    uint32_t err = 0;

    APP_TIMER_INIT(ATTR_TIMER_PRESCALER, ATTR_TIMER_COUNT, ATTR_TIMER_QUEUE_LENGTH, false);

    EC(app_timer_create(&mainTimerMsId, APP_TIMER_MODE_REPEATED, ble_timer_dispatch));

    EC(app_timer_start(mainTimerMsId, APP_TIMER_TICKS(ATTR_TIMER_TICK_MS, ATTR_TIMER_PRESCALER), NULL));
}


void timer_tick_handler(void)
{
    if (timeSinceLastHandlerCalled > 0){
        //For now, just make the LEDs blink purple every 2 seconds
        if (timeSinceLastBlink % timeBetweenBlinks == 0){
            led_red_on();
            led_blue_on();
            timeSinceLastBlink = 0;
        } else {
            led_red_off();
            led_blue_off();
        }
        timeSinceLastHandlerCalled = 0;
    }
}
