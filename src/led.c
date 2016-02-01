#include <led.h>

void led_red_on(void)
{
  NRF_GPIO->DIRSET = LED_RED;
  NRF_GPIO->OUTCLR = LED_RED;
}

void led_red_off(void)
{
  NRF_GPIO->DIRSET = LED_RED;
  NRF_GPIO->OUTSET = LED_RED;
}

void led_green_on(void)
{
  NRF_GPIO->DIRSET = LED_GREEN;
  NRF_GPIO->OUTCLR = LED_GREEN;
}

void led_green_off(void)
{
  NRF_GPIO->DIRSET = LED_GREEN;
  NRF_GPIO->OUTSET = LED_GREEN;
}

void led_blue_on(void)
{
  NRF_GPIO->DIRSET = LED_BLUE;
  NRF_GPIO->OUTCLR = LED_BLUE;
}

void led_blue_off(void)
{
  NRF_GPIO->DIRSET = LED_BLUE;
  NRF_GPIO->OUTSET = LED_BLUE;
}

void led_white_on(void)
{
  NRF_GPIO->DIRSET = LED_WHITE;
  NRF_GPIO->OUTCLR = LED_WHITE;
}

void led_white_off(void)
{
  NRF_GPIO->DIRSET = LED_WHITE;
  NRF_GPIO->OUTSET = LED_WHITE;
}
