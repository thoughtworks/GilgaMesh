#include <timer.h>
#include <pn532.h>

#define MAX_TIMERS 4
#define QUEUE_SIZE 1
#define ATTR_TIMER_PRESCALER 0

#define MS_RATE_TO_MANAGE_NFC 100
#define MS_RATE_TO_TURN_OFF_ALL_LEDS 100

static app_timer_id_t mainTimerMsId;
static app_timer_id_t ledTimerMsId;
static app_timer_id_t nfcTimerMsId;

int time_since_last_handler_called = 0;

int timeSinceLastBlink = 0;
int timeBetweenBlinks = 2000;
#define BLINK_TIMER_TICK_MS 200

void turn_off_leds(void *context) {
    led_white_off();
}

void nfc_event_handler(void *context) {
    manage_badge_reading();
}

void timer_dispatch(void *context) {
    timeSinceLastBlink += BLINK_TIMER_TICK_MS;
    time_since_last_handler_called += BLINK_TIMER_TICK_MS;
}

void timer_initialize(void) {
    APP_TIMER_INIT(ATTR_TIMER_PRESCALER, MAX_TIMERS, QUEUE_SIZE, false);

    //EC(app_timer_create(&mainTimerMsId, APP_TIMER_MODE_REPEATED, timer_dispatch));

    //EC(app_timer_create(&nfcTimerMsId, APP_TIMER_MODE_REPEATED, nfc_event_handler));
    //EC(app_timer_create(&ledTimerMsId, APP_TIMER_MODE_REPEATED, turn_off_leds));

    //EC(app_timer_start(mainTimerMsId, APP_TIMER_TICKS(BLINK_TIMER_TICK_MS, ATTR_TIMER_PRESCALER), NULL));
    //EC(app_timer_start(nfcTimerMsId, APP_TIMER_TICKS(MS_RATE_TO_MANAGE_NFC, ATTR_TIMER_PRESCALER), NULL));
    //EC(app_timer_start(ledTimerMsId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_ALL_LEDS, ATTR_TIMER_PRESCALER), NULL));
}

void timer_tick_handler(void) {
    if (time_since_last_handler_called > 0) {
        if (timeSinceLastBlink % timeBetweenBlinks == 0) {
            led_red_on();
            led_blue_on();
            timeSinceLastBlink = 0;
        } else {
            led_red_off();
            led_blue_off();
        }
    }
    time_since_last_handler_called = 0;
}
