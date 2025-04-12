#ifndef VMOPTS_H
#define VMOPTS_H

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_HEAP_INITIAL 4096
#define DEFAULT_HEAP_MAX 20480

typedef struct vm_options {
    size_t classpath_len;
    const char **classpath;
    uint8_t flags;
    size_t argc;
    const char **argv;
    size_t heap_init;
    size_t heap_max;
} VmOptions;

void PrintHelp(void);
void PrintVersionInformation(void);

extern const VmOptions DEFAULT_OPTIONS;

#endif
