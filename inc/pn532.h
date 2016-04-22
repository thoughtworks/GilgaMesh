#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "app_util_platform.h"
#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_uart.h"

#include "led.h"

typedef void (* uart_event_handler) (uint8_t rx_byte);

typedef enum {
    DOWN,
    NO_PARAM,
    NOT_WRITTEN_80,
    RF_NOT_CONFIG,
    NOT_WRITTEN_40_10,
    RF_MAX_NOT_CONFIG,
    SETUP_DONE
} setup_state_t;

bool id_exists_in_response(uint8_t *response, size_t response_length);
unsigned short get_id(uint8_t *response, size_t response_length);
void manage_badge_reading();
void initialize_uart_nfc();
setup_state_t get_setup_state();
void setup();
void setup_state_machine();
void in_list_passive_target();
void power_down(void);
