#include "terminal.h"

#include <app_scheduler.h>
#include <SEGGER_RTT.h>
#include <stdlib.h>

#include "system/log.h"

#define ENTER_KEY 0xA
#define ESCAPE_KEY 0x1b

static bool is_terminal_initialized = false;
static char readBuffer[TERMINAL_BUFFER_SIZE] = {0};
static int readBufferIndex = 0;

void terminal_putstring(char* string) {
  if(is_terminal_initialized) {
    NRF_LOG_RAW_INFO("%s", string);
  }
}

static uint8_t terminal_get() {
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

void terminal_execute_command(char* command) {
  char *parsedCommand[MAX_ARGUMENTS + 1];
  uint8_t numberOfArguments = find_arguments(parsedCommand, command);

  command_execute(parsedCommand, numberOfArguments);
  terminal_putstring("\r\n");
}

static void reset_terminal_input() {
  memset(readBuffer, 0, TERMINAL_BUFFER_SIZE);
  readBufferIndex = 0;
}

void terminal_process_input(void) {
  if (!is_terminal_initialized) { return; }

  uint8_t nextCharacter = terminal_get();
  if(nextCharacter != 0 && in_user_input_mode()) {
    if (nextCharacter == ENTER_KEY || readBufferIndex >= TERMINAL_BUFFER_SIZE - 1) {
      set_user_input_mode(false);
      append_char_to_buffer('\0');
      terminal_execute_command(readBuffer);
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