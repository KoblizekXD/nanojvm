#ifndef NANOJVM_COMMON_COMMONS_H
#define NANOJVM_COMMON_COMMONS_H

#include <stddef.h>
#include <stdint.h>

#if defined(__STDC_HOSTED__) && __STDC_HOSTED__ == 0
    #define FREESTANDING
#endif

typedef void *(*njvmMalloc)(size_t size);
typedef void (*njvmFree)(void *ptr);
typedef void *(*njvmRealloc)(void *ptr, size_t size);

#define LOG_STATUS_INFO  32
#define LOG_STATUS_WARN  33
#define LOG_STATUS_ERROR 31
#define LOG_STATUS_DEBUG 37

#define LOG_STATEMENT_ALLOC_FAILED "Failure in memory allocation: "

// strrchr implementation
char *StrRCharSearch(const char *s, int c);

#if defined(ENABLE_LOGGING_IN_FREESTANDING_ENV) || !defined(FREESTANDING)

#ifdef DEBUG

#define __FILENAME__ (StrRCharSearch(__FILE__, '/') ? StrRCharSearch(__FILE__, '/') + 1 : __FILE__)

void log_message(int status, const char *file, int line, const char *format, ...);

#define info(message, ...) log_message(LOG_STATUS_INFO, __FILENAME__, __LINE__, message, ##__VA_ARGS__)
#define warn(message, ...) log_message(LOG_STATUS_WARN, __FILENAME__, __LINE__, message, ##__VA_ARGS__)
#define error(message, ...) log_message(LOG_STATUS_ERROR, __FILENAME__, __LINE__, message, ##__VA_ARGS__)
#define debug(message, ...) log_message(LOG_STATUS_DEBUG, __FILENAME__, __LINE__, message, ##__VA_ARGS__)
#else
void log_message(int status, const char *format, ...);

#define info(message, ...) log_message(LOG_STATUS_INFO, message, ##__VA_ARGS__)
#define warn(message, ...) log_message(LOG_STATUS_WARN, message, ##__VA_ARGS__)
#define error(message, ...) log_message(LOG_STATUS_ERROR, message, ##__VA_ARGS__)
#define debug(message, ...)
#endif

#else

#define info(message, ...)
#define warn(message, ...)
#define error(message, ...)
#define debug(message, ...)

#endif

/*
 * The following functions read data from a pointer and advance the pointer.
 * They are used to read data from a byte array in big-endian order.
 * The pointer is passed as a pointer to a pointer, so that the function can
 * modify the original pointer.
 */

uint64_t get64(void **ptr);
uint32_t get32(void **ptr);
uint16_t get16(void **ptr);
uint8_t get8(void **ptr);

/**
 * Advances a pointer by n bytes.
 * @param ptr pointer to advance
 * @param n number of bytes to advance by
 */
#define skip(ptr, n) ((ptr) = (void *)((uintptr_t)(ptr) + (n)))

/**
 * Offsets a pointer by n bytes and returns the new pointer(without modifying the old one).
 * @param ptr pointer to offset
 * @param n number of bytes to offset by
 */
#define offset(ptr, n) ((void *)((uintptr_t)(ptr) + (n)))

// strlen implementation
size_t StringLength(const char *s);
// djb2 string hash function
uint32_t StringHash(const char *s);
// djb2 string hash function with length limit
uint32_t StringHashEx(const char *s, size_t length);
// memcpy implementation
void *MemoryCopy(void *dest, const void *src, size_t n);
// memset implementation
void *MemorySet(void *s, int c, size_t n);
// memcmp implementation
int MemoryCompare(const void *s1, const void *s2, size_t n);

#endif //NANOJVM_COMMON_COMMONS_H