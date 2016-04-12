#include <timer.h>
#include <pn532.h>
#include <buzzer.h>

#define MAX_TIMERS 4
#define QUEUE_SIZE 1
#define ATTR_TIMER_PRESCALER 0

#define MS_RATE_TO_MANAGE_NFC 100
#define MS_RATE_TO_TURN_OFF_ALL_LEDS 100

static app_timer_id_t feedbackOffTimerId;
static app_timer_id_t nfcTimerId;

void turn_off_user_feedback(void *context) {
    led_white_off();
    buzzer_off();
}

void nfc_event_handler(void *context) {
    manage_badge_reading();
}

void timer_initialize(void) {
    APP_TIMER_INIT(ATTR_TIMER_PRESCALER, MAX_TIMERS, QUEUE_SIZE, false);

    EC(app_timer_create(&nfcTimerId, APP_TIMER_MODE_REPEATED, nfc_event_handler));
    EC(app_timer_start(nfcTimerId, APP_TIMER_TICKS(MS_RATE_TO_MANAGE_NFC, ATTR_TIMER_PRESCALER), NULL));

    EC(app_timer_create(&feedbackOffTimerId, APP_TIMER_MODE_REPEATED, turn_off_user_feedback));
    EC(app_timer_start(feedbackOffTimerId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_ALL_LEDS, ATTR_TIMER_PRESCALER), NULL));
}
