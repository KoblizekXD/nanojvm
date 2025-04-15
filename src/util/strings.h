#ifndef NANOJVM_UTIL_STRINGS_H
#define NANOJVM_UTIL_STRINGS_H

#include <stddef.h>

#define StringEquals(STR1, STR2) (strcmp(STR1, STR2) == 0)

int StartsWith(const char *str, const char *prefix);
int EndsWith(const char *str, const char *suffix);
size_t ComputeHash(const char *str); // Compute hash of string with djb2

#endif
