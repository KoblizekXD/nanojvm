#ifndef NANOJVM_MEMORY_UTILS_H
#define NANOJVM_MEMORY_UTILS_H

#include <stddef.h>
#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int strcmp(const char *s1, const char *s2);

#endif // NANOJVM_MEMORY_UTILS_H
