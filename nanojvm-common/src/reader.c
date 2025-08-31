#include <commons.h>

uint8_t get8(void **ptr) {
    unsigned char *p = *ptr;
    const uint8_t value = p[0];
    *ptr = p + 1;
    return value;
}

uint16_t get16(void **ptr) {
    unsigned char *p = *ptr;
    const uint16_t value = (uint16_t)p[0] << 8 |
                     (uint16_t)p[1];
    *ptr = p + 2;
    return value;
}

uint32_t get32(void **ptr) {
    unsigned char *p = *ptr;
    const uint32_t value = (uint32_t)p[0] << 24 |
                     (uint32_t)p[1] << 16 |
                     (uint32_t)p[2] << 8  |
                     (uint32_t)p[3];
    *ptr = p + 4;
    return value;
}

uint64_t get64(void **ptr) {
    unsigned char *p = *ptr;
    const uint64_t value = (uint64_t)p[0] << 56 |
                     (uint64_t)p[1] << 48 |
                     (uint64_t)p[2] << 40 |
                     (uint64_t)p[3] << 32 |
                     (uint64_t)p[4] << 24 |
                     (uint64_t)p[5] << 16 |
                     (uint64_t)p[6] << 8  |
                     (uint64_t)p[7];
    *ptr = p + 8;
    return value;
}