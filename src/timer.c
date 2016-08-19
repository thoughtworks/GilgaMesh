#include "timer.h"

#include <app_timer_appsh.h>
#include <app_scheduler.h>

#include "message_types/heartbeat_message.h"

#define QUEUE_SIZE 10 // what should this be? "1" doesn't work, FYI
#define SCHED_MAX_EVENT_DATA_SIZE (sizeof(ble_evt_t)*2)
#define SCHED_QUEUE_SIZE 20 // what should this be???
#define APP_TIMER_PRESCALER         0                                  /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE     4                                  /**< Size of timer operation queues. */

#define MS_RATE_TO_BROADCAST_HEARTBEAT 3000

APP_TIMER_DEF(heartbeatTimer);

static void heartbeat_timer_handler(void *context) {
  app_sched_event_put(NULL, 0, send_heartbeat_message);
}

void turn_on_heartbeats() {
  EC(app_timer_start(heartbeatTimer, APP_TIMER_TICKS(MS_RATE_TO_BROADCAST_HEARTBEAT, APP_TIMER_PRESCALER), NULL));
}

void turn_off_heartbeats() {
  EC(app_timer_stop(heartbeatTimer));
}

void timer_initialize(void) {
  APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);

  EC(app_timer_create(&heartbeatTimer, APP_TIMER_MODE_REPEATED, heartbeat_timer_handler));
}
