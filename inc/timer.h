#pragma once

#if defined(TESTING) && !defined(TIMER_TEST)

#else

#include <app_timer.h>
#include <error.h>
#include <logger.h>

#endif

void timer_initialize();
void turn_on_user_feedback();
void turn_on_buzzer_feedback_timeout();
void turn_on_buffer_clear_timeout();
void turn_off_buffer_clear_timeout();
void turn_on_send_vote_timeout();
void turn_off_send_vote_timeout();
void turn_on_nfc_reader();
void turn_on_heartbeats();
void turn_off_heartbeats();


