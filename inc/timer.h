#pragma once

#if defined(TESTING) && !defined(TIMER_TEST)

#else

#include <app_timer.h>
#include <led.h>
#include <error.h>
#include <logger.h>


void timer_initialize(void);
void timer_tick_handler(void);

#endif