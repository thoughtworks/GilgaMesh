#pragma once

#include <app_scheduler.h>
#include <app_timer_appsh.h>
#include <app_timer.h>
#include <stdint.h>
#include "error.h"

#define APP_TIMER_PRESCALER         0                                  /**< Value of the RTC1 PRESCALER register. */

void timer_initialize();
void create_repeated_timer(const app_timer_id_t *timer_id);
void create_single_shot_timer(const app_timer_id_t *timer_id);
void start_timer(const app_timer_id_t *timer_id, uint32_t ms_to_execute, app_sched_event_handler_t callback);
void stop_timer(const app_timer_id_t *timer_id);


