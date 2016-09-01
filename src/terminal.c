#include "terminal.h"
#include "app/rtc.h"
#include "app/feedback.h"
#include <stdlib.h>

#ifdef NRF_LOG_USES_RTT
#define COMMAND_TERMINATOR 0xA
#else
#define COMMAND_TERMINATOR 0xD
#endif

static bool is_terminal_initialized = false;

void terminal_put(const uint8_t character) {
  if(is_terminal_initialized) {
#ifdef NRF_LOG_USES_RTT
    NRF_LOG_PRINTF("%c", character);
#else
    simple_uart_put(character);
#endif
  }
}

void terminal_putstring(char* string) {
  if(is_terminal_initialized) {
#ifdef NRF_LOG_USES_RTT
    NRF_LOG_PRINTF(string);
#else
    simple_uart_putstring(string);
#endif
  }
}

uint8_t terminal_get() {
  uint8_t retval = 0;

  if(is_terminal_initialized) {
#ifdef NRF_LOG_USES_RTT
    while (!NRF_LOG_HAS_INPUT()) { } // Emulate blocking UART behavior
    NRF_LOG_READ_INPUT((char *) &retval);
#else
    retval = simple_uart_get();
#endif
  }
  return retval;
}

void terminal_get_with_timeout(uint8_t* character) {
  if(is_terminal_initialized) {
#ifdef NRF_LOG_USES_RTT
    if (NRF_LOG_HAS_INPUT()) {
      NRF_LOG_READ_INPUT((char *) character);
    }
#else
    simple_uart_get_with_timeout(TERMINAL_READ_TIMEOUT_US, character);
#endif
  }
  return;
}

static void print_prompt() {
  terminal_putstring(TERMINAL_PROMPT);
}

static void send_command_mode_ack() {
  terminal_putstring(COMMAND_MODE_ACK);
}

void terminal_clear() {
#ifndef NRF_LOG_USES_RTT
  terminal_put(27);
  terminal_putstring("[2J"); //Clear Screen
  terminal_put(27);
  terminal_putstring("[H"); //Cursor to Home
#else
  terminal_putstring("\n\r--------------------------------------------------------\n\r");
#endif
}

void terminal_initialize(void) {
#ifndef NRF_LOG_USES_RTT
  simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
#endif
  is_terminal_initialized = true;

  char mainVersion[4];
  char subVersion[4];
  terminal_clear();

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
  //terminal_putstring(dfu_device_name_with_id());
  terminal_putstring("\r\n-----------| ESC to pause into command mode |-----------\r\n");

}

bool terminal_initialized()
{
   return is_terminal_initialized;
}

static void suspend_app_timers() {
  rtc_sysclock_timer_suspend();
#ifdef IS_PROD_BOARD
  suspend_feedback_timer();
#endif
}

static void resume_app_timers() {
#ifdef IS_PROD_BOARD
  resume_feedback_timer();
#endif
  rtc_sysclock_timer_resume();
}

static void terminal_read_command(char* readBuffer, uint8_t readBufferLength, uint8_t offset)
{
  if (!is_terminal_initialized) return;

  suspend_app_timers();

  uint8_t byteBuffer;
  uint8_t counter = offset;

	//Read in an infinite loop until \r is recognized
	while (true)
	{
		//Read from terminal
		byteBuffer = terminal_get();

		//BACKSPACE
		if (byteBuffer == 127)
		{
			if (counter > 0)
			{
#ifndef NRF_LOG_USES_RTT
        terminal_put(byteBuffer); // echo the backspace
#endif
				readBuffer[counter - 1] = 0;
				counter--;
			}
		}
		else //ALL OTHER CHARACTERS
		{

#ifndef NRF_LOG_USES_RTT
      terminal_put(byteBuffer); // echo the entered character
#endif

			if (byteBuffer == COMMAND_TERMINATOR || counter >= readBufferLength || counter >= READ_BUFFER_SIZE) // if enter or out of buffer
			{
				readBuffer[counter] = '\0';
				break;
			}
			else
			{
            readBuffer[counter] = byteBuffer;
         }

			counter++;
		}
	}

  resume_app_timers();
}

static bool read_terminal(char* readBuffer) {
  terminal_read_command(readBuffer, READ_BUFFER_SIZE - 1, 0);
  if (readBuffer[0] == 0) { return false; } // empty command

  return true;
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

void execute_command() {
  char readBuffer[READ_BUFFER_SIZE] = {0};
  char *parsedCommand[MAX_ARGUMENTS + 1]; // first element is the command

  if (!read_terminal(readBuffer)) { // empty command -> clear screen
    terminal_clear();
    return;
  }

  uint8_t numberOfArguments = find_arguments(parsedCommand, readBuffer);

  command_execute(parsedCommand, numberOfArguments);
  terminal_putstring("\r\n");
}


void terminal_process_input(void) {
  if (!is_terminal_initialized) { return; }

  uint8_t leadingCharacter = 0;
  terminal_get_with_timeout(&leadingCharacter);
  if (leadingCharacter == 0x1b) { // ESC pressed
    print_prompt();
    execute_command();
  } else if (leadingCharacter == 0x7E) {
    send_command_mode_ack();
    execute_command();
  }
}