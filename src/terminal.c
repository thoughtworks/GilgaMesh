#include <terminal.h>
#include <string.h>

#define READ_BUFFER_SIZE 128
#define TERMINAL_PROMPT "\r\n#> "
#define MAX_ARGUMENTS 3

static bool is_terminal_initialized = false;

static void print_prompt() {
  simple_uart_putstring((const uint8_t*) TERMINAL_PROMPT);
}

void terminal_initialize(void)
{
  simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);

  simple_uart_put(27);
  simple_uart_putstring((const uint8_t*) "[2J"); //Clear Screen
  simple_uart_put(27);
  simple_uart_putstring((const uint8_t*) "[H"); //Cursor to Home

   simple_uart_putstring((const uint8_t*) "    __  __        _        __  __        _\r\n");
   simple_uart_putstring((const uint8_t*) "   |  \\/  |___ __| |_ _  _|  \\/  |___ __| |_\r\n");
   simple_uart_putstring((const uint8_t*) "   | |\\/| / -_|_-< ' \\ || | |\\/| / -_|_-< ' \\ \r\n");
   simple_uart_putstring((const uint8_t*) "   |_|  |_\\___/__/_||_\\_, |_|  |_\\___/__/_||_| ");
   simple_uart_putstring((const uint8_t*) "v");
   simple_uart_putstring((const uint8_t*) int_to_string(APP_VERSION_MAIN));
   simple_uart_putstring((const uint8_t*) ".");
   simple_uart_putstring((const uint8_t*) int_to_string(APP_VERSION_SUB));
   simple_uart_putstring((const uint8_t*) "\r\n");
   simple_uart_putstring((const uint8_t*) "                        |__/\r\n");


  simple_uart_putstring((const uint8_t*) "  compile date: ");
  simple_uart_putstring((const uint8_t*) __DATE__);
  simple_uart_putstring((const uint8_t*) " ");
  simple_uart_putstring((const uint8_t*) __TIME__);

  simple_uart_putstring((const uint8_t*) ", nRF51s ");
  simple_uart_putstring((const uint8_t*) dfu_device_name_with_id());
  simple_uart_putstring((const uint8_t*) "\r\n------------------------------------------------------\r\n");

  is_terminal_initialized = true;
}

bool terminal_initialized()
{
   return is_terminal_initialized;
}

static void ReadlineUART(char* readBuffer, uint8_t readBufferLength, uint8_t offset)
{
  if (!is_terminal_initialized) return;

   uint8_t byteBuffer;
   uint8_t counter = offset;

	//Read in an infinite loop until \r is recognized
	while (true)
	{
		//Read from terminal
		byteBuffer = simple_uart_get();

		//BACKSPACE
		if (byteBuffer == 127)
		{
			if (counter > 0)
			{
				//Output Backspace
				simple_uart_put(byteBuffer);

				readBuffer[counter - 1] = 0;
				counter--;
			}
		}
		else //ALL OTHER CHARACTERS
		{
			simple_uart_put(byteBuffer); // echo the entered character

			if (byteBuffer == 0xD || counter >= readBufferLength || counter >= READ_BUFFER_SIZE) // if enter or out of buffer
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
}

static bool read_terminal(char* readBuffer) {
   ReadlineUART(readBuffer, READ_BUFFER_SIZE - 1, 0);
   if (readBuffer[0] == NULL) { return false; } // empty command

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

static void print_parsed_command(char** parsedCommandArray, uint8_t numberOfArgs) {
   simple_uart_putstring("\r\nCOMMAND: ");
   simple_uart_putstring(parsedCommandArray[0]);
   simple_uart_putstring("\r\n");

   for (int i = 1; i < numberOfArgs; ++i) {
      simple_uart_putstring("ARG");
      simple_uart_putstring(int_to_string(i));
      simple_uart_putstring(": ");
      simple_uart_putstring(parsedCommandArray[i]);
      simple_uart_putstring("\r\n");
   }
}

void terminal_process_input(void)
{
   if (!is_terminal_initialized) { return; }
   print_prompt();

   char readBuffer[READ_BUFFER_SIZE] = {0};
   char *parsedCommand[MAX_ARGUMENTS + 1]; // first element is the command

   if(!read_terminal(readBuffer)) {
      return;
   }

   uint8_t numberOfArguments = find_arguments(parsedCommand, readBuffer);

   print_parsed_command(parsedCommand, numberOfArguments);
}