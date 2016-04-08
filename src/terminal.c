#include <terminal.h>
#include <string.h>

#define READ_BUFFER_SIZE 256
#define TERMINAL_PROMPT "\r\n#> "
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

  simple_uart_putstring((const uint8_t*) "------------------------------------------------------\r\n");

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
			//ALL OTHER CHARACTERS
		}
		else
		{

			//Display entered character in terminal
			simple_uart_put(byteBuffer);

			if (byteBuffer == '\r' || counter >= readBufferLength || counter >= READ_BUFFER_SIZE)
			{
				readBuffer[counter] = '\0';
				simple_uart_putstring("\r\n");
				break;
			}
			else
			{
				memcpy(readBuffer + counter, &byteBuffer, sizeof(uint8_t));
			}

			counter++;
		}
	}
}

void terminal_process_input(void)
{
  char readBuffer[READ_BUFFER_SIZE] = {0};

   if(!is_terminal_initialized) { return; }

  if(simple_uart_get_with_timeout(1, (uint8_t*) readBuffer)) {
    simple_uart_put(readBuffer[0]);
  }

  //Read line from uart if input doesn't start with enter
  if(readBuffer[0] != '\r') {
    //ReadlineUART(readBuffer, READ_BUFFER_SIZE, 1);
  }
  else {
    print_prompt();
  }
}