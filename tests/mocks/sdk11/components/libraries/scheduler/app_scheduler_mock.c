#include <app_scheduler.h>
#include "cmocka_includes.h"

uint32_t app_sched_init(uint16_t max_event_size, uint16_t queue_size, void * p_evt_buffer) { }
void app_sched_execute(void) { }
uint32_t app_sched_event_put(void *p_event_data, uint16_t event_data_size, app_sched_event_handler_t handler) {
  check_expected(p_event_data);
  return NRF_SUCCESS;
}
