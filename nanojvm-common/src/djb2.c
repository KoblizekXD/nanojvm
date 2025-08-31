#include <commons.h>

uint32_t StringHash(const char *s)
{
    uint32_t hash = 5381;
    int c;

    while ((c = (unsigned char) *s++)) {
        hash = (hash << 5) + hash + c;
    }
    return hash;
}

uint32_t StringHashEx(const char *s, const size_t length)
{
    uint32_t hash = 5381;
    size_t i = 0;
    int c;

    while (i < length && ((c = (unsigned char) *s++))) {
        hash = (hash << 5) + hash + c;
        i++;
    }
    return hash;
}