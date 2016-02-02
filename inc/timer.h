#pragma once

#include <app_timer.h>
#include <led.h>

#define ATTR_TIMER_TICK_MS 200
#define ATTR_TIMER_COUNT 1
#define ATTR_TIMER_QUEUE_LENGTH 1
#define ATTR_TIMER_PRESCALER 0

void timer_initialize(void);
void timer_tick_handler(void);
