#include <nrf_log.h>
#include "cmocka_includes.h"

uint32_t log_rtt_init(void);

void log_rtt_printf(int terminal_index, char * format_msg, ...) {

}

void log_rtt_write_string(int terminal_index, int num_args, ...) {

}

void log_rtt_write_hex(int terminal_index, uint32_t value) {

}

void log_rtt_write_hex_char(int terminal_index, uint8_t value) {

}

int log_rtt_has_input(void){
   return mock_type(int);
}

uint32_t log_rtt_read_input(char* p_char) {
   return mock_type(uint32_t);
}

