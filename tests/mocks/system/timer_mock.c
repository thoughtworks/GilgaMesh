#include "system/timer.h"
#include "cmocka_includes.h"


void timer_initialize(void) { }

void create_repeated_timer(const app_timer_id_t *timer_id) { }

void create_single_shot_timer(const app_timer_id_t *timer_id) {
  check_expected_ptr(timer_id);
}

void start_timer(const app_timer_id_t *timer_id, uint32_t ms_to_execute, app_sched_event_handler_t callback) { }
