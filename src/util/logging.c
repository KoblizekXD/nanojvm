#include <util/logging.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef DEBUG

void log_message(int status, const char *file, int line, const char *format, ...)
{
    printf("\033[%dm[%s:%d][%s]: ", status, file, line, status == LOG_STATUS_INFO ? "info" : status == LOG_STATUS_WARN ? "warning" : status == LOG_STATUS_ERROR ? "error" : "debug");
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\033[0m\n");
}

#else

void log_message(int status, const char *format, ...)
{
    printf("\033[%dm[%s]: ", status, status == LOG_STATUS_INFO ? "info" : status == LOG_STATUS_WARN ? "warn" : status == LOG_STATUS_ERROR ? "error" : "debug");
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\033[39m\n");
}

#endif
