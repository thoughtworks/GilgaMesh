#include <stdint.h>

void display_general_user_feedback(void *data, uint16_t dataLength);
void display_vote_recorded_feedback(void);
void display_group_value_change_feedback(void);
void display_catastrophic_failure_feedback(void);
void turn_off_buzzer_feedback(void *data, uint16_t dataLength);