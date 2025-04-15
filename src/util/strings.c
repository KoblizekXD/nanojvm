#include <util/strings.h>
#include <stddef.h>
#include <string.h>

int StartsWith(const char *str, const char *prefix)
{
    if (!str || !prefix) return 0;
    while (*prefix) {
        if (*str++ != *prefix++) return 0;
    }
    return 1;
}

int EndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix) return 0;
    
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    
    if (suffix_len > str_len) return 0;
    return memcmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

size_t ComputeHash(const char *str)
{
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
