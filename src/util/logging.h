#ifndef NANOJVM_LOGGING_H
#define NANOJVM_LOGGING_H

#define LOG_STATUS_INFO  32
#define LOG_STATUS_WARN  33
#define LOG_STATUS_ERROR 31
#define LOG_STATUS_DEBUG 90

#define LOG_STATEMENT_ALLOC_FAILED "Failure in memory allocation: "

#ifdef DEBUG
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

void log_message(int status, const char *file, int line, const char *format, ...);

#define info(message, ...) log_message(LOG_STATUS_INFO, __FILENAME__, __LINE__, message, ##__VA_ARGS__)
#define warn(message, ...) log_message(LOG_STATUS_WARN, __FILENAME__, __LINE__, message, ##__VA_ARGS__)
#define error(message, ...) log_message(LOG_STATUS_ERROR, __FILENAME__, __LINE__, message, ##__VA_ARGS__)
#define debug(message, ...) log_message(LOG_STATUS_DEBUG, message, ##__VA_ARGS__)
#else
void log_message(int status, const char *format, ...);

#define info(message, ...) log_message(LOG_STATUS_INFO, message, ##__VA_ARGS__)
#define warn(message, ...) log_message(LOG_STATUS_WARN, message, ##__VA_ARGS__)
#define error(message, ...) log_message(LOG_STATUS_ERROR, message, ##__VA_ARGS__)
#define debug(message, ...)
#endif

#endif
