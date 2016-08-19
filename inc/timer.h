#pragma once

#if defined(TESTING) && !defined(TIMER_TEST)

#else

#include <app_timer.h>
#include <error.h>
#include <logger.h>

#endif

void timer_initialize();
void turn_on_heartbeats();
void turn_off_heartbeats();


