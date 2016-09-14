#pragma once

#include <stdint.h>
#include "error.h"

#ifndef TESTING
#include <app_timer.h>
#include <app_scheduler.h>
#include <app_timer_appsh.h>
#endif

#define APP_TIMER_PRESCALER         0                                  /**< Value of the RTC1 PRESCALER register. */
#define SYSTEM_TIMER_NODE_SIZE          32

typedef struct system_timer_t { uint32_t data[SYSTEM_TIMER_NODE_SIZE]; } system_timer_t;
typedef system_timer_t* system_timer_id_t;
typedef void (*system_sched_event_handler_t)(void * p_event_data, uint16_t event_size);

#define SYS_TIMER_DEF(timer_id)                                  \
    static system_timer_t timer_id##_data = { {0} };                  \
    static const system_timer_id_t timer_id = &timer_id##_data

void timer_initialize();
void create_repeated_timer(const system_timer_id_t *timer_id);
void create_single_shot_timer(const system_timer_id_t *timer_id);
void start_timer(const system_timer_id_t *timer_id, uint32_t ms_to_execute, system_sched_event_handler_t callback);
void stop_timer(const system_timer_id_t *timer_id);


