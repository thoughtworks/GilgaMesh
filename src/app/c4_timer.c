#include "timer.h"
#include "c4_timer.h"

#include <app_timer_appsh.h>
#include <app_scheduler.h>

#include "ble.h"
#include "feedback.h"
#include "gatt.h"

#define MS_RATE_TO_MANAGE_NFC 100
#define MS_RATE_TO_DISPLAY_USER_FEEDBACK 100
#define MS_RATE_TO_TURN_OFF_BUZZER_FEEDBACK 600
#define MS_RATE_TO_CLEAR_VOTE_BUFFER 3000
#define MS_RATE_TO_SEND_VOTE 5000
#define MS_RATE_TO_UPDATE_SYSTEM_CLOCK 1

static app_timer_id_t displayFeedbackTimerId;
static app_timer_id_t feedbackOffTimerId;
static app_timer_id_t nfcTimerId;
static app_timer_id_t clearVoteBufferTimerId;
static app_timer_id_t sendVoteTimerId;
static app_timer_id_t clockTimerId;


void display_user_feedback_handler(void *context) {
  app_sched_event_put(NULL, 0, display_general_user_feedback);
}

void turn_off_buzzer_feedback_handler(void *context) {
  app_sched_event_put(NULL, 0, turn_off_buzzer_feedback);
}

void nfc_event_handler(void *context) {
  app_sched_event_put(NULL, 0, manage_badge_reading);
}

void turn_on_buzzer_feedback_timeout() {
  EC(app_timer_start(feedbackOffTimerId, APP_TIMER_TICKS(MS_RATE_TO_TURN_OFF_BUZZER_FEEDBACK, ATTR_TIMER_PRESCALER), NULL));
}

void turn_on_user_feedback() {
  EC(app_timer_start(displayFeedbackTimerId, APP_TIMER_TICKS(MS_RATE_TO_DISPLAY_USER_FEEDBACK, ATTR_TIMER_PRESCALER), NULL));
}

void clear_vote_buffer_timer_handler(void *context) {
  app_sched_event_put(NULL, 0, clear_vote_buffer);
}

void turn_on_buffer_clear_timeout() {
  EC(app_timer_start(clearVoteBufferTimerId, APP_TIMER_TICKS(MS_RATE_TO_CLEAR_VOTE_BUFFER, ATTR_TIMER_PRESCALER), NULL));
}

void turn_off_buffer_clear_timeout() {
  EC(app_timer_stop(clearVoteBufferTimerId));
}

void send_vote_timer_handler(void *context) {
  app_sched_event_put(NULL, 0, get_first_vote);
}

void turn_on_send_vote_timeout() {
  EC(app_timer_start(sendVoteTimerId, APP_TIMER_TICKS(MS_RATE_TO_SEND_VOTE, ATTR_TIMER_PRESCALER), NULL));
}

void turn_off_send_vote_timeout() {
  EC(app_timer_stop(sendVoteTimerId));
}

void turn_on_nfc_reader() {
  EC(app_timer_start(nfcTimerId, APP_TIMER_TICKS(MS_RATE_TO_MANAGE_NFC, ATTR_TIMER_PRESCALER), NULL));
}

void clock_timer_handler(void *context) {
  rtc_periodic_update_handler();
}

void c4_timer_initialize(void) {

#ifdef NFC_DEBUG
  EC(app_timer_create(&nfcTimerId, APP_TIMER_MODE_REPEATED, nfc_event_handler));

  EC(app_timer_create(&feedbackOffTimerId, APP_TIMER_MODE_SINGLE_SHOT, turn_off_buzzer_feedback_handler));

  EC(app_timer_create(&clearVoteBufferTimerId, APP_TIMER_MODE_SINGLE_SHOT, clear_vote_buffer_timer_handler));

  EC(app_timer_create(&sendVoteTimerId, APP_TIMER_MODE_REPEATED, send_vote_timer_handler));
#endif

  EC(app_timer_create(&displayFeedbackTimerId, APP_TIMER_MODE_REPEATED, display_user_feedback_handler));

  EC(app_timer_create(&clockTimerId, APP_TIMER_MODE_REPEATED, clock_timer_handler));

  EC(app_timer_start(clockTimerId, APP_TIMER_TICKS(MS_RATE_TO_UPDATE_SYSTEM_CLOCK, ATTR_TIMER_PRESCALER), NULL));
}