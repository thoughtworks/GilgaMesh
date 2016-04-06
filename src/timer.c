#include <timer.h>
#include <pn532.h>

static app_timer_id_t mainTimerMsId;
uint16_t timeSinceLastHandlerCalled = 0;
uint32_t timeSinceLastBlink = 0;
uint16_t timeBetweenNfc = 1000;

uint32_t masterClockTick = 0;
uint16_t masterIntervalLength = 100;
uint16_t maxIntervalLength = 1000;

bool inNewTimeFrame();

bool timeToCreateNewInterval();

void ble_timer_dispatch(void *context) {
    timeSinceLastHandlerCalled += ATTR_TIMER_TICK_MS;
    masterClockTick += ATTR_TIMER_TICK_MS;
}


void timer_initialize(void) {
    APP_TIMER_INIT(ATTR_TIMER_PRESCALER, ATTR_TIMER_COUNT, ATTR_TIMER_QUEUE_LENGTH, false);

    EC(app_timer_create(&mainTimerMsId, APP_TIMER_MODE_REPEATED, ble_timer_dispatch));

    EC(app_timer_start(mainTimerMsId, APP_TIMER_TICKS(ATTR_TIMER_TICK_MS, ATTR_TIMER_PRESCALER), NULL));
}

//check error with below code to see if in error for logging here :D
//if (get_setup_state() == SETUP_DONE) {
//if (first_response_received)
//    current_nfc_state = TAG_PRESENCE_UNKNOWN;
//else {
//    led_red_on();
//}
//}

void timer_tick_handler(void) {
    if (timeSinceLastHandlerCalled > 0 ) {

        if (inNewTimeFrame()) {
            manage_badge_reading();

        }
        if (timeToCreateNewInterval()) {
            masterClockTick = 0;
        }

    }

    timeSinceLastHandlerCalled = 0;
}

bool timeToCreateNewInterval() { return masterClockTick % 1000 == 0; }

bool inNewTimeFrame() { return masterClockTick % masterIntervalLength == 0; }

