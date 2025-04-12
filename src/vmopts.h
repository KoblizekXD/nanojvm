#ifndef VMOPTS_H
#define VMOPTS_H

#include <stddef.h>
#include <stdint.h>

typedef struct vm_options {
    size_t classpath_len;
    const char **classpath;
    uint8_t flags;
    size_t argc;
    const char **argv;
} VmOptions;

void PrintHelp(void);
void PrintVersionInformation(void);

#endif
