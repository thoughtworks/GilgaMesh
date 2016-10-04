#include "terminal.h"

#include <app_scheduler.h>
#include <app_uart.h>
#include <SEGGER_RTT.h>

#include "system/log.h"
#include "boards.h"
#include "error.h"

#define ENTER_KEY 0xA
#define CARRIAGE_RETURN_KEY 0xD
#define ESCAPE_KEY 0x1b

static bool is_terminal_initialized = false;
static char readBuffer[READ_BUFFER_SIZE] = {0};
static int readBufferIndex = 0;

static void uart_event_handle(app_uart_evt_t * p_event);

void terminal_putstring(char* string) {
  if(is_terminal_initialized) {
    NRF_LOG_RAW_INFO("%s", string);
  }
}

uint8_t terminal_get() {
  uint8_t charValue = 0;
  if(is_terminal_initialized) {
    if (SEGGER_RTT_HasKey()) {
      charValue = (uint8_t)SEGGER_RTT_GetKey();
    }
  }
  return charValue;
}

static void print_prompt() {
  terminal_putstring(TERMINAL_PROMPT);
}

static bool char_ends_message(char someCharacter) {
  return someCharacter == ENTER_KEY || someCharacter == CARRIAGE_RETURN_KEY;
}

static void uart_initialize() {
  uint32_t                     err_code;
  const app_uart_comm_params_t comm_params =
          {
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
}

void log_to_uart(char *logMessage, uint16_t messageLength) {
  for (int i = 0; i < messageLength; i++) {
    while (app_uart_put((uint8_t) logMessage[i]) != NRF_SUCCESS);
    if (logMessage[i] == '\0') break;
  }
}

void terminal_initialize(void) {
  is_terminal_initialized = true;

  char mainVersion[4];
  char subVersion[4];
  terminal_putstring("\n\r--------------------------------------------------------\n\r");

  terminal_putstring("    __  __        _        __  __        _\r\n");
  terminal_putstring("   |  \\/  |___ __| |_ _  _|  \\/  |___ __| |_\r\n");
  terminal_putstring("   | |\\/| / -_|_-< ' \\ || | |\\/| / -_|_-< ' \\ \r\n");
  terminal_putstring("   |_|  |_\\___/__/_||_\\_, |_|  |_\\___/__/_||_| ");
  terminal_putstring("v");
  terminal_putstring(itoa(APP_VERSION_MAIN, mainVersion, 10));
  terminal_putstring(".");
  terminal_putstring(itoa(APP_VERSION_SUB, subVersion, 10));
  terminal_putstring("\r\n");
  terminal_putstring("                      |__/\r\n");


  terminal_putstring("  compile date: ");
  terminal_putstring(__DATE__);
  terminal_putstring(" ");
  terminal_putstring(__TIME__);

  terminal_putstring(", nRF51s ");
  terminal_putstring("\r\n-----------| ESC to pause into command mode |-----------\r\n");

  uart_initialize();

}

static uint8_t find_arguments(char** parsedCommandArray, char* readBuffer) {
  uint8_t i = 0;
  uint8_t argumentCount = 0;
  char *token = strtok(readBuffer, " ");

  while (token != NULL && argumentCount <= MAX_ARGUMENTS) {
    parsedCommandArray[i++] = token;
    token = strtok(NULL, " ");
    argumentCount++;
  }

  return argumentCount;
}

static void append_char_to_buffer(char new_character) {
  readBuffer[readBufferIndex++] = new_character;
}

static void act_upon_user_input(char *command) {
  char *parsedCommand[MAX_ARGUMENTS + 1];
  uint8_t numberOfArguments = find_arguments(parsedCommand, command);

  command_execute(parsedCommand, numberOfArguments);
  terminal_putstring("\r\n");
}

void terminal_execute_command(char* string) {
  act_upon_user_input(string);
}

static void reset_terminal_input() {
  memset(readBuffer, 0, READ_BUFFER_SIZE);
  readBufferIndex = 0;
}

void terminal_process_input(void) {
  if (!is_terminal_initialized) { return; }

  uint8_t nextCharacter = terminal_get();
  if(nextCharacter != 0 && in_user_input_mode()) {
    if (nextCharacter == ENTER_KEY || readBufferIndex >= READ_BUFFER_SIZE - 1) {
      set_user_input_mode(false);
      append_char_to_buffer('\0');
      act_upon_user_input(readBuffer);
      reset_terminal_input();
    }
    else
      append_char_to_buffer(nextCharacter);
  }
  else if (nextCharacter == ESCAPE_KEY) {
    print_prompt();
    set_user_input_mode(true);
  }
}

static void uart_event_handle(app_uart_evt_t * p_event) {
  uint8_t inputChar;
  switch (p_event->evt_type) {
    case APP_UART_DATA:
      while (app_uart_get(&inputChar) != NRF_SUCCESS);

      if (char_ends_message(inputChar) || readBufferIndex >= READ_BUFFER_SIZE - 1) {
        append_char_to_buffer('\0');
        static char bufferCopy[READ_BUFFER_SIZE];
        memcpy(bufferCopy, readBuffer, READ_BUFFER_SIZE);
        EC(app_sched_event_put(bufferCopy, READ_BUFFER_SIZE, act_upon_user_input));
        reset_terminal_input();

      } else {
        append_char_to_buffer(inputChar);
      }
      break;
    default:
      break;
  }
}