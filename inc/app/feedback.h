#pragma once

#include <stdint.h>

void feedback_initialize(void);
void suspend_feedback_timer(void);
void resume_feedback_timer(void);
void display_successful_start_up_feedback(void);
void display_vote_recorded_feedback(void);
void display_group_value_change_feedback(void);
void display_catastrophic_failure_feedback(void);
