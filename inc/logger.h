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

const char* getBleEventNameString(uint16_t bleEventId);

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