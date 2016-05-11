#include "cmocka_includes.h"
#include "bsp.h"

uint32_t bsp_init(uint32_t type, uint32_t ticks_per_100ms, bsp_event_callback_t callback) {
   check_expected(type);
   check_expected(ticks_per_100ms);

   return mock_type(uint32_t);
}

uint32_t bsp_buttons_state_get(uint32_t * p_buttons_state) {

   return mock_type(uint32_t);
}

uint32_t bsp_button_is_pressed(uint32_t button, bool * p_state) {
   check_expected(button);

   return mock_type(uint32_t);
}

uint32_t bsp_event_to_button_action_assign(uint32_t button, bsp_button_action_t action, bsp_event_t event) {
   check_expected(button);

   return mock_type(uint32_t);
} 

uint32_t bsp_indication_set(bsp_indication_t indicate) {

   return mock_type(uint32_t);
}

uint32_t bsp_indication_text_set(bsp_indication_t indicate, const char * p_text) {

   return mock_type(uint32_t);
}

uint32_t bsp_buttons_enable(void) {

   return mock_type(uint32_t);
}

uint32_t bsp_buttons_disable(void) {

   return mock_type(uint32_t);
}

uint32_t bsp_wakeup_buttons_set(uint32_t wakeup_buttons) {
   check_expected(wakeup_buttons);

   return mock_type(uint32_t);
}


