#include <stddef.h>

int MemoryCompare(const void *s1, const void *s2, const size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (((const unsigned char *)s1)[i] != ((const unsigned char *)s2)[i]) {
            return ((const unsigned char *)s1)[i] - ((const unsigned char *)s2)[i];
        }
    }
    return 0;
}