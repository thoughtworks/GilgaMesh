#include <conversion.h>
#include <stdio.h>

char* int_to_string(uint16_t i)
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 1];
  char *p = buf + INT_DIGITS;/* points to terminating '\0' */
  do {
    *--p = '0' + (i % 10);
    i /= 10;
  } while (i != 0);
  return p;
}
