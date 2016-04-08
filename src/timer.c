#include <timer.h>
#include <pn532.h>

#define MAX_TIMERS 3
#define QUEUE_SIZE 1
#define ATTR_TIMER_PRESCALER 0
#define MS_RATE_TO_TURN_OFF_ALL_LEDS 100

#define ATTR_TIMER_TICK_MS 100
#define BUZZER_TIMER_TICKS 100
#define MS_RATE_TO_MANAGE_NFC 100

static app_timer_id_t mainTimerMsId;
static app_timer_id_t ledTimerMsId;
static app_timer_id_t buzzerTimerMsId;
static app_timer_id_t nfcTimerMsId;

int timeSinceLastBuzz = 0;

void turn_off_leds(void *context) {
    led_white_off();
}

void nfc_event_handler(void *context) {
    manage_badge_reading();
}

void buzzer_timer_dispatch(void *context) {
    timeSinceLastBuzz += ATTR_TIMER_TICK_MS;
}

void timer_initialize(void) {
    APP_TIMER_INIT(ATTR_TIMER_PRESCALER, MAX_TIMERS, QUEUE_SIZE, false);

    //EC(app_timer_create(&nfcTimerMsId, APP_TIMER_MODE_REPEATED, nfc_event_handler));
    //EC(app_timer_create(&ledTimerMsId, APP_TIMER_MODE_REPEATED, turn_off_leds));

    //EC(app_timer_create(&buzzerTimerMsId, APP_TIMER_MODE_REPEATED, buzzer_timer_dispatch));

    //EC(app_timer_start(nfcTimerMsId, APP_TIMER_TICKS(MS_RATE_TO_MANAGE_NFC, ATTR_TIMER_PRESCALER), NULL));
    //EC(app_timer_start(ledTimerMsId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_ALL_LEDS, ATTR_TIMER_PRESCALER), NULL));

    //EC(app_timer_start(buzzerTimerMsId, APP_TIMER_TICKS(BUZZER_TIMER_TICKS, ATTR_TIMER_PRESCALER), NULL));
}
