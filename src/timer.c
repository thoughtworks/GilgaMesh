#include <timer.h>
#include <ble.h>
#include <pn532.h>
#include <buzzer.h>
#include <gatt.h>
#include <app_timer_appsh.h>
#include <app_scheduler.h>

#define MAX_TIMERS 4
#define QUEUE_SIZE 1
#define ATTR_TIMER_PRESCALER 0
#define SCHED_MAX_EVENT_DATA_SIZE (sizeof(ble_evt_t)*2)
#define SCHED_QUEUE_SIZE 10

#define MS_RATE_TO_MANAGE_NFC 100
#define MS_RATE_TO_TURN_OFF_ALL_USER_FEEDBACK 50
#define MS_RATE_TO_BROADCAST_HEARTBEAT 3000

static app_timer_id_t feedbackOffTimerId;
static app_timer_id_t nfcTimerId;
static app_timer_id_t heartbeatTimerId;

void turn_off_user_feedback(void *context) {
  led_white_off();
  buzzer_off();
  display_connection_status();
}

void nfc_event_handler(void *context) {
  manage_badge_reading();
}

void heartbeat_timer_handler(void *context) {
  app_sched_event_put(NULL, 0, broadcast_heartbeat);
}

void turn_off_voting_notification() {
  EC(app_timer_start(feedbackOffTimerId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_ALL_USER_FEEDBACK, ATTR_TIMER_PRESCALER), NULL));
}

void timer_initialize(void) {
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
  APP_TIMER_APPSH_INIT(ATTR_TIMER_PRESCALER, MAX_TIMERS, QUEUE_SIZE, false);

#ifdef NFC_DEBUG
  EC(app_timer_create(&nfcTimerId, APP_TIMER_MODE_REPEATED, nfc_event_handler));
  EC(app_timer_start(nfcTimerId, APP_TIMER_TICKS(MS_RATE_TO_MANAGE_NFC, ATTR_TIMER_PRESCALER), NULL));
#endif

  EC(app_timer_create(&feedbackOffTimerId, APP_TIMER_MODE_SINGLE_SHOT, turn_off_user_feedback));

  EC(app_timer_create(&heartbeatTimerId, APP_TIMER_MODE_REPEATED, heartbeat_timer_handler));
  EC(app_timer_start(heartbeatTimerId, APP_TIMER_TICKS(MS_RATE_TO_BROADCAST_HEARTBEAT, ATTR_TIMER_PRESCALER), NULL));
}
