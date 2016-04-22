#include <timer.h>
#include <pn532.h>
#include <buzzer.h>
#include <gatt.h>

#define MAX_TIMERS 4
#define QUEUE_SIZE 1
#define ATTR_TIMER_PRESCALER 0

#define MS_RATE_TO_MANAGE_NFC 100
#define MS_RATE_TO_TURN_OFF_ALL_USER_FEEDBACK 100
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
  broadcast_heartbeat();
}

//void turn_on_mesh_user_feedback(void *context) {
//  EC(app_timer_stop(feedbackOffTimerId));
//  display_connection_status();
//  EC(app_timer_start(feedbackOffTimerId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_ALL_USER_FEEDBACK, ATTR_TIMER_PRESCALER), NULL));
//}

void turn_off_voting_notification() {
  EC(app_timer_start(feedbackOffTimerId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_ALL_USER_FEEDBACK, ATTR_TIMER_PRESCALER), NULL));
}

void timer_initialize(void) {
  APP_TIMER_INIT(ATTR_TIMER_PRESCALER, MAX_TIMERS, QUEUE_SIZE, false);

#ifdef NFC_DEBUG
  EC(app_timer_create(&nfcTimerId, APP_TIMER_MODE_REPEATED, nfc_event_handler));
  EC(app_timer_start(nfcTimerId, APP_TIMER_TICKS(MS_RATE_TO_MANAGE_NFC, ATTR_TIMER_PRESCALER), NULL));
#endif

  EC(app_timer_create(&feedbackOffTimerId, APP_TIMER_MODE_SINGLE_SHOT, turn_off_user_feedback));
//  EC(app_timer_start(feedbackOffTimerId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_ALL_USER_FEEDBACK, ATTR_TIMER_PRESCALER), NULL));
//
//  EC(app_timer_create(&meshFeedbackOnTimerId, APP_TIMER_MODE_REPEATED, turn_on_mesh_user_feedback));
//  EC(app_timer_start(meshFeedbackOnTimerId, APP_TIMER_TICKS(MS_RATE_TO_TURN_ON_MESH_USER_FEEDBACK, ATTR_TIMER_PRESCALER), NULL));

  EC(app_timer_create(&heartbeatTimerId, APP_TIMER_MODE_REPEATED, heartbeat_timer_handler));
  EC(app_timer_start(heartbeatTimerId, APP_TIMER_TICKS(MS_RATE_TO_BROADCAST_HEARTBEAT, ATTR_TIMER_PRESCALER), NULL));
}
