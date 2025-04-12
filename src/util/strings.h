#ifndef NANOJVM_UTIL_STRINGS_H
#define NANOJVM_UTIL_STRINGS_H

#define StringEquals(STR1, STR2) (strcmp(STR1, STR2) == 0)

int StartsWith(const char *str, const char *prefix);
int EndsWith(const char *str, const char *suffix);

#endif
