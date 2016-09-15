#include "timer.h"
#include "app/c4_timer.h"

#include <app_timer_appsh.h>
#include <app_scheduler.h>

#include "ble.h"
#include "gatt.h"

#define MS_RATE_TO_MANAGE_NFC 100
#define MS_RATE_TO_UPDATE_SYSTEM_CLOCK 1

static app_timer_id_t nfcTimerId;
static app_timer_id_t clockTimerId;


void nfc_event_handler(void *context) {
  app_sched_event_put(NULL, 0, manage_badge_reading);
}

void turn_on_nfc_reader() {
  EC(app_timer_start(nfcTimerId, APP_TIMER_TICKS(MS_RATE_TO_MANAGE_NFC, ATTR_TIMER_PRESCALER), NULL));
}

void clock_timer_handler(void *context) {
  rtc_periodic_update_handler();
}

void c4_timer_initialize(void) {

#ifdef BOARD_SEEED
  EC(app_timer_create(&nfcTimerId, APP_TIMER_MODE_REPEATED, nfc_event_handler));
#endif

  EC(app_timer_create(&clockTimerId, APP_TIMER_MODE_REPEATED, clock_timer_handler));

  EC(app_timer_start(clockTimerId, APP_TIMER_TICKS(MS_RATE_TO_UPDATE_SYSTEM_CLOCK, ATTR_TIMER_PRESCALER), NULL));
}
