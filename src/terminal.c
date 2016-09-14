#include "terminal.h"

#include <stdlib.h>
#include <SEGGER_RTT.h>
#include "system/log.h"

#define ENTER_KEY 0xA
#define ESCAPE_KEY 0x1b

static bool is_terminal_initialized = false;
static char readBuffer[READ_BUFFER_SIZE] = {0};
static int readBufferIndex = 0;

void terminal_putstring(char* string) {
  if(is_terminal_initialized) {
    //SEGGER_RTT_printf("%s", string);
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
  terminal_putstring("                        |__/\r\n");


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

static void act_upon_user_input() {
  readBuffer[readBufferIndex] = '\0';

  char *parsedCommand[MAX_ARGUMENTS + 1];
  uint8_t numberOfArguments = find_arguments(parsedCommand, readBuffer);

  command_execute(parsedCommand, numberOfArguments);
  terminal_putstring("\r\n");
}

static void reset_terminal_input() {
  memset(readBuffer, 0, READ_BUFFER_SIZE);
  readBufferIndex = 0;
}

void terminal_process_input(void) {
  if (!is_terminal_initialized) { return; }

  uint8_t nextCharacter = terminal_get();
  if(nextCharacter != 0 && in_user_input_mode()) {
    if (nextCharacter == ENTER_KEY|| readBufferIndex >= READ_BUFFER_SIZE-1) {
      set_user_input_mode(false);
      act_upon_user_input();
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