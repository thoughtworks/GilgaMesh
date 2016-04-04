#include "cmocka_includes.h"

void print_led_state(const char* color, const char* state)
{
#ifndef NO_LOG_OUTPUT
   printf("|*%s*| %s\n", color, state);
#endif
}

void led_red_on(void)
{
   print_led_state("RED", "ON");
}

void led_red_off(void)
{
   print_led_state("RED", "OFF");
}

void led_green_on(void)
{
   print_led_state("GREEN", "ON");
}

void led_green_off(void)
{
   print_led_state("GREEN", "OFF");
}

void led_blue_on(void)
{
   print_led_state("BLUE", "ON");
}

void led_blue_off(void)
{
   print_led_state("BLUE", "OFF");
}

void led_white_on(void)
{
   print_led_state("WHITE", "ON");
}

void led_white_off(void)
{
   print_led_state("RED", "OFF");
}



