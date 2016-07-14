#pragma once

#define TRACE_BUFFER_SIZE 500

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
//TODO: we need to take this out later!!!!!
#include <stdio.h>

#ifdef _MSC_VER
#include <string.h>
#define __FILE_S__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILE_S__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

typedef enum {UART_COMMUNICATION, LOG_LINE, LOG_MESSAGE_ONLY} LogType;
typedef enum {NO_ERROR, COMMAND_NOT_FOUND, ARGUMENTS_WRONG} UartErrorType;

void log_f(bool printLine, const char* file, int32_t line, const char* message, ...);
void uart_error_f(UartErrorType type);

//The Logger implements the Terminal Listener
//bool TerminalCommandHandler(string commandName, vector<string> commandArgs);

//Functions for resolving error codes
const char* getNrfErrorString(uint32_t nrfErrorCode);
const char* getHciErrorString(uint8_t hciErrorCode);
const char* getBleEventNameString(uint16_t bleEventId);
const char* getGattStatusErrorString(uint16_t gattStatusCode);
const char* getPstorageStatusErrorString(uint16_t operationCode);

/*
 * Define macros that rewrite the log function calls to the Logger
 * */

//Used for UART communication between node and attached pc
#ifdef ENABLE_UART

#define uart(tag, message, ...) log_f(false, __FILE_S__, __LINE__, message, ##__VA_ARGS__)
#define uart_error(type) uart_error_f(type)

#else //ENABLE_UART

#define uart(...) do{}while(0)
#define uart_error(...) do{}while(0)

#endif

#define trace(message, ...) log_f(false, __FILE_S__, __LINE__, message, ##__VA_ARGS__)
#define log(message, ...) log_f(true, __FILE_S__, __LINE__, message, ##__VA_ARGS__)

#if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1

#define LOG_TERMINAL_NORMAL         (0)
#define LOG_TERMINAL_ERROR          (1)
#define LOG_TERMINAL_INPUT          (0)

/**@brief Function for initializing the SEGGER RTT logger.
 *
 * @details See <a href="https://www.segger.com/jlink-rtt.html" target="_blank">segger.com</a>
 *          for information about SEGGER Real Time Transfer (RTT).
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use the macro @ref NRF_LOG_INIT instead.
 *
 * @retval     NRF_SUCCESS     If initialization was successful.
 * @retval     NRF_ERROR       Otherwise.
 */
uint32_t log_rtt_init(void);

/**@brief Function for writing a printf string using RTT.
 *
 * @details The printf implementation in SEGGER's RTT is more efficient than
 * the standard implementation. However, printf requires more processor time
 * than other logging functions. Therefore, applications that require logging
 * but need it to interfere as little as possible with the execution, should
 * avoid using printf.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_PRINTF
 * - @ref NRF_LOG_PRINTF_DEBUG
 * - @ref NRF_LOG_PRINTF_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   format_msg      Printf format string.
 */
void log_rtt_printf(int terminal_index, char * format_msg, ...);

/**@brief Function for writing a string using RTT.
 *
 * @details The string to write must be null-terminated, but the null termination will not be stored
 *          in the ring buffer.
 *          The impact of running this function should be very low compared to writing to UART.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG
 * - @ref NRF_LOG_DEBUG
 * - @ref NRF_LOG_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   num_args        Number of arguments.
 */
void log_rtt_write_string(int terminal_index, int num_args, ...);

/**@brief Function for writing an integer as HEX using RTT.
 *
 * The output data is formatted as, for example, 0x89ABCDEF.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX
 * - @ref NRF_LOG_HEX_DEBUG
 * - @ref NRF_LOG_HEX_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   value           Integer value to be printed as HEX.
 */
void log_rtt_write_hex(int terminal_index, uint32_t value);

/**@brief Function for writing a single character as HEX using RTT.
 *
 * The output string is formatted as, for example, AA.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX_CHAR
 * - @ref NRF_LOG_HEX_CHAR_DEBUG
 * - @ref NRF_LOG_HEX_CHAR_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   value           Character to print as HEX.
 */
void log_rtt_write_hex_char(int terminal_index, uint8_t value);

/**@brief Function for checking if data is available in the input buffer.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use @ref NRF_LOG_HAS_INPUT instead.
 *
 * @retval      1 If characters are available to read.
 * @retval      0 If no characters are available.
 */
int log_rtt_has_input(void);

/**@brief Function for reading one character from the input buffer.
 *
 * @param[out] p_char  Pointer where to store the character.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use @ref NRF_LOG_READ_INPUT instead.
 *
 * @retval      NRF_SUCCESS If the character was read out.
 * @retval      NRF_ERROR_INVALID_DATA If no character could be read.
 */
uint32_t log_rtt_read_input(char* p_char);

#define NRF_LOG_INIT()                  log_rtt_init()                                                                          /*!< Initialize the module. */

#define NRF_LOG_PRINTF(...)             log_rtt_printf(LOG_TERMINAL_NORMAL, ##__VA_ARGS__)                                      /*!< Print a log message using printf. */
#define NRF_LOG_PRINTF_DEBUG(...)       log_rtt_printf(LOG_TERMINAL_NORMAL, ##__VA_ARGS__)                                      /*!< If DEBUG is set, print a log message using printf. */
#define NRF_LOG_PRINTF_ERROR(...)       log_rtt_printf(LOG_TERMINAL_ERROR, ##__VA_ARGS__)                                       /*!< Print a log message using printf to the error stream. */

#define NRF_LOG(...)                    log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< Print a log message. The input string must be null-terminated. */
#define NRF_LOG_DEBUG(...)              log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< If DEBUG is set, print a log message. The input string must be null-terminated. */
#define NRF_LOG_ERROR(...)              log_rtt_write_string(LOG_TERMINAL_ERROR, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)       /*!< Print a log message to the error stream. The input string must be null-terminated. */

#define NRF_LOG_HEX(val)                log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< Log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_DEBUG(val)          log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< If DEBUG is set, log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_ERROR(val)          log_rtt_write_hex(LOG_TERMINAL_ERROR, val)                                              /*!< Log an integer as HEX value to the error stream (example output: 0x89ABCDEF). */

#define NRF_LOG_HEX_CHAR(val)           log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< Log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< If DEBUG is set, log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_rtt_write_hex_char(LOG_TERMINAL_ERROR, val)                                         /*!< Log a character as HEX value to the error stream (example output: AA). */

#define NRF_LOG_HAS_INPUT()             log_rtt_has_input()                                                                     /*!< Check if the input buffer has unconsumed characters. */
#define NRF_LOG_READ_INPUT(p_char)      log_rtt_read_input(p_char)                                                              /*!< Consume a character from the input buffer. */

#else

#define NRF_LOG_INIT()
#define NRF_LOG_PRINTF(...)
#define NRF_LOG_PRINTF_DEBUG(...)
#define NRF_LOG_PRINTF_ERROR(...)

#define NRF_LOG(...)                    log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< Print a log message. The input string must be null-terminated. */
#define NRF_LOG_DEBUG(...)              log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< If DEBUG is set, print a log message. The input string must be null-terminated. */
#define NRF_LOG_ERROR(...)              log_rtt_write_string(LOG_TERMINAL_ERROR, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)       /*!< Print a log message to the error stream. The input string must be null-terminated. */

#define NRF_LOG_HEX(val)                log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< Log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_DEBUG(val)          log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< If DEBUG is set, log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_ERROR(val)          log_rtt_write_hex(LOG_TERMINAL_ERROR, val)                                              /*!< Log an integer as HEX value to the error stream (example output: 0x89ABCDEF). */

#define NRF_LOG_HEX_CHAR(val)           log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< Log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< If DEBUG is set, log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_rtt_write_hex_char(LOG_TERMINAL_ERROR, val)                                         /*!< Log a character as HEX value to the error stream (example output: AA). */

#define NRF_LOG_HAS_INPUT()             log_rtt_has_input()                                                                     /*!< Check if the input buffer has unconsumed characters. */
#define NRF_LOG_READ_INPUT(p_char)      log_rtt_read_input(p_char)                                                              /*!< Consume a character from the input buffer. */

#endif // (NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1