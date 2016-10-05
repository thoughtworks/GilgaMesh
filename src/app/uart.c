#include "app/uart.h"

#include <app_scheduler.h>
#include <app_uart.h>
#include <terminal.h>

#include "system/log.h"
#include "boards.h"
#include "error.h"

#define ENTER_KEY 0xA
#define CARRIAGE_RETURN_KEY 0xD

static bool is_uart_initialized = false;
static char readBuffer[UART_BUFFER_SIZE] = {0};
static int readBufferIndex = 0;

static void uart_event_handle(app_uart_evt_t * p_event);

static bool char_ends_message(char someCharacter) {
  return someCharacter == ENTER_KEY || someCharacter == CARRIAGE_RETURN_KEY;
}

void uart_initialize() {
  uint32_t err_code;
  const app_uart_comm_params_t comm_params = {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud38400
  };

  APP_UART_INIT(&comm_params,
                uart_event_handle,
                APP_IRQ_PRIORITY_LOW,
                err_code);
  EC(err_code);
  is_uart_initialized = true;
}

void log_to_uart(char *logMessage, uint16_t messageLength) {
  if (!is_uart_initialized) { return; }

  for (int i = 0; i < messageLength; i++) {
    while (app_uart_put((uint8_t) logMessage[i]) != NRF_SUCCESS);
    if (logMessage[i] == '\0') break;
  }
}

static void append_char_to_buffer(char new_character) {
  readBuffer[readBufferIndex++] = new_character;
}

static void reset_uart_input() {
  memset(readBuffer, 0, UART_BUFFER_SIZE);
  readBufferIndex = 0;
}

static void uart_event_handle(app_uart_evt_t * p_event) {
  uint8_t inputChar;
  switch (p_event->evt_type) {
    case APP_UART_DATA:
      while (app_uart_get(&inputChar) != NRF_SUCCESS);

      if (char_ends_message(inputChar) || readBufferIndex >= UART_BUFFER_SIZE - 1) {
        append_char_to_buffer('\0');
        static char bufferCopy[UART_BUFFER_SIZE];
        memcpy(bufferCopy, readBuffer, UART_BUFFER_SIZE);
        EC(app_sched_event_put(bufferCopy, UART_BUFFER_SIZE, terminal_execute_command));
        reset_uart_input();

      } else {
        append_char_to_buffer(inputChar);
      }
      break;
    default:
      break;
  }
}