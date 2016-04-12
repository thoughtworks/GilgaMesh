#include <led.h>

#ifdef BOARD_SEEED
bool PRODUCTION_BOARD = true;
#else
bool PRODUCTION_BOARD = false;
#endif

void led_red_on(void)
{
  NRF_GPIO->DIRSET = LED_RED;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTSET = LED_RED;
  else NRF_GPIO->OUTCLR = LED_RED;
}

void led_red_off(void)
{
  NRF_GPIO->DIRSET = LED_RED;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTCLR = LED_RED;
  else NRF_GPIO->OUTSET = LED_RED;
}

void led_green_on(void)
{
  NRF_GPIO->DIRSET = LED_GREEN;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTSET = LED_GREEN;
  else NRF_GPIO->OUTCLR = LED_GREEN;
}

void led_green_off(void)
{
  NRF_GPIO->DIRSET = LED_GREEN;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTCLR = LED_GREEN;
  else NRF_GPIO->OUTSET = LED_GREEN;
}

void led_blue_on(void)
{
  NRF_GPIO->DIRSET = LED_BLUE;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTSET = LED_BLUE;
  else NRF_GPIO->OUTCLR = LED_BLUE;
}

void led_blue_off(void)
{
  NRF_GPIO->DIRSET = LED_BLUE;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTCLR = LED_BLUE;
  else NRF_GPIO->OUTSET = LED_BLUE;
}

void led_white_on(void)
{
  NRF_GPIO->DIRSET = LED_WHITE;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTSET = LED_WHITE;
  else NRF_GPIO->OUTCLR = LED_WHITE;
}

void led_white_off(void)
{
  NRF_GPIO->DIRSET = LED_WHITE;
  if (PRODUCTION_BOARD) NRF_GPIO->OUTCLR = LED_WHITE;
  else NRF_GPIO->OUTSET = LED_WHITE;
}
