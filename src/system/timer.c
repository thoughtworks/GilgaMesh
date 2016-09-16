#include "system/timer.h"
#include "error.h"

#define APP_TIMER_OP_QUEUE_SIZE     20                                  /**< Size of timer operation queues. */

void timer_initialize(void) {
  APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);
}

static void generic_timer_handler(void *context) {
  app_sched_event_put(NULL, 0, (app_sched_event_handler_t)context);
}

void create_repeated_timer(const system_timer_id_t *timer_id) {
  EC(app_timer_create(timer_id, APP_TIMER_MODE_REPEATED, generic_timer_handler));
}

void create_single_shot_timer(const system_timer_id_t *timer_id) {
  EC(app_timer_create(timer_id, APP_TIMER_MODE_SINGLE_SHOT, generic_timer_handler));
}

void start_timer(const system_timer_id_t *timer_id, uint32_t ms_to_execute, app_sched_event_handler_t callback) {
  EC(app_timer_start(*timer_id, APP_TIMER_TICKS(ms_to_execute, APP_TIMER_PRESCALER), callback));
}

void stop_timer(const system_timer_id_t *timer_id) {
  EC(app_timer_stop(*timer_id));
}



