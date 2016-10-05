#include "system/timer.h"

#include "cmocka_includes.h"


static bool shouldExecuteCallback = false;

bool timer_initialize(void) {
  return true;
}

ret_code_t create_repeated_timer(const system_timer_id_t *timer_id) {
  check_expected(timer_id);
  return NRF_SUCCESS;
}

ret_code_t create_single_shot_timer(const system_timer_id_t *timer_id) {
  check_expected(timer_id);
  return NRF_SUCCESS;
}

ret_code_t start_timer(const system_timer_id_t *timer_id, uint32_t ms_to_execute, system_sched_event_handler_t callback) {
  check_expected(timer_id);
  check_expected(ms_to_execute);
  if (shouldExecuteCallback) {
    callback(NULL, NULL);
  }
  return NRF_SUCCESS;
}

void execute_timer_callback(bool value) {
  shouldExecuteCallback = value;
}

void stop_timer(const system_timer_id_t *timer_id) {
  check_expected(timer_id);
}