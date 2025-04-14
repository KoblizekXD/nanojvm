#ifndef NANOJVM_MEM_MEMUTILS_H
#define NANOJVM_MEM_MEMUTILS_H

#include <stddef.h>

typedef struct meminfo {
    size_t total_usage;
    size_t heap_allocated;
} MemoryInformation;

MemoryInformation GetMemoryState(void);

#endif
