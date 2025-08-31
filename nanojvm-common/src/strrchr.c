#include <stddef.h>

char *StrRCharSearch(const char *s, const int c) {
    const char *last = NULL;
    const unsigned char ch = (unsigned char)c;

    while (*s != '\0') {
        if ((unsigned char)*s == ch) {
            last = s;
        }
        s++;
    }

    if (ch == '\0') {
        return (char *)s;
    }

    return (char *)last;
}
