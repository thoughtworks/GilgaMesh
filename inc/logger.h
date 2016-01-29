#pragma once

#include <stdint.h>
#include <stdbool.h>

#define TRACE_BUFFER_SIZE 500

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


#ifdef ENABLE_LOGGING

#define trace(message, ...) log_f(false, __FILE_S__, __LINE__, message, ##__VA_ARGS__)
#define log(message, ...) log_f(true, __FILE_S__, __LINE__, message, ##__VA_ARGS__)

#else //ENABLE_LOGGING

#define trace(...) do{}while(0)
#define log(...) do{}while(0)

#endif //ENABLE_LOGGING
