#include <commons.h>

void *MemoryCopy(void *dest, const void *src, const size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

void *MemorySet(void *s, const int c, const size_t n)
{
    unsigned char *p = s;
    const unsigned char byte = (unsigned char)c;

    for (size_t i = 0; i < n; i++) {
        p[i] = byte;
    }

    return s;
}