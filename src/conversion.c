#include <conversion.h>

uint8_t hex_digit_to_uint8(char digit) {
  int retval = 0;

  for (int i = 0 ; i < 16 ; i++) {
    if( HEX_DIGITS[i] == digit ) {
      retval = i;
    }
  }

  return (uint8_t) retval;
}
