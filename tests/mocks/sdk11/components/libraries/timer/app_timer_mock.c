#include <app_timer.h>
#include "cmocka_includes.h"

uint32_t app_timer_init(uint32_t prescaler,
                        uint8_t op_queues_size,
                        void *p_buffer,
                        app_timer_evt_schedule_func_t evt_schedule_func) {
  return mock_type(uint32_t);
}

uint32_t app_timer_create(app_timer_id_t const *p_timer_id,
                          app_timer_mode_t mode,
                          app_timer_timeout_handler_t timeout_handler) {
  return mock_type(uint32_t);
}

uint32_t app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void *p_context) {
  return mock_type(uint32_t);
}

uint32_t app_timer_stop(app_timer_id_t timer_id) {
  return mock_type(uint32_t);
}

uint32_t app_timer_stop_all(void) {
  return mock_type(uint32_t);
}

uint32_t app_timer_cnt_get(uint32_t *p_ticks) {
  return mock_type(uint32_t);
}

uint32_t app_timer_cnt_diff_compute(uint32_t ticks_to,
                                    uint32_t ticks_from,
                                    uint32_t *p_ticks_diff) {
  return mock_type(uint32_t);
}

uint32_t app_timer_evt_schedule(app_timer_timeout_handler_t timeout_handler,
                                void *p_context) {
  return mock_type(uint32_t);
}