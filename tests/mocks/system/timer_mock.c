#include "system/timer.h"

#include "cmocka_includes.h"


static bool shouldExecuteCallback = false;

void timer_initialize(void) { }

void create_repeated_timer(const system_timer_id_t *timer_id) {
  check_expected(timer_id);
}

void create_single_shot_timer(const system_timer_id_t *timer_id) {
  check_expected(timer_id);
}

void start_timer(const system_timer_id_t *timer_id, uint32_t ms_to_execute, system_sched_event_handler_t callback) {
  check_expected(timer_id);
  check_expected(ms_to_execute);
  if (shouldExecuteCallback) {
    callback(NULL, NULL);

  }
}

void execute_timer_callback(bool value) {
  shouldExecuteCallback = value;
}

void stop_timer(const system_timer_id_t *timer_id) {
  check_expected(timer_id);
}