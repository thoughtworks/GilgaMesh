#ifndef PN532_H
#define PN532_H

#ifdef __cplusplus
extern "C" {
#endif

/*lint ++flb "Enter library region" */

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    UART_ERROR,
    UART_RECEIVED
} uart_event_t;

typedef struct {
    uart_event_t event_t;
    uint8_t data;
} uart_event;

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

setup_state_t get_setup_state();
void setup();

void nfcEventHandler(uart_event *event);

void set_parameter();
void write_80();
void config_rf();
void write_40_10();
void config_rf_max();

void wakeup();
void powerdown();
void setup_mifare_ultralight();
unsigned short in_list_passive_target();
void uart_115200_config(uint8_t rts_pin_number, uint8_t txd_pin_number,
						uint8_t cts_pin_number, uint8_t rxd_pin_number, uart_event_handler event_handler);
void uart_put_char(uint8_t character);
void gobble_number_of_bytes(int number);
void send_preamble_and_start();
void send_direction();
void send_postamble();
uint8_t uart_get();
bool uart_get_with_timeout();
bool tag_is_present();
void get_ack();
void in_data_exchange(uint8_t address, uint8_t other);

void retarget_init();


#ifdef __cplusplus
}
#endif

/*lint --flb "Leave library region" */
#endif
