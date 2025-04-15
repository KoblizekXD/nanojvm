#ifndef VMOPTS_H
#define VMOPTS_H

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_HEAP_INITIAL 4096
#define DEFAULT_HEAP_MAX 20480

#define OPTION_DISABLE_JVM_LOOKUP (1 << 0)
#define OPTION_DISABLE_CLINIT (1 << 1)
#define OPTION_DISABLE_INIT (1 << 2)

typedef struct vm_options {
    size_t classpath_len;
    char **classpath;
    uint8_t flags;
    size_t argc;
    char **argv;
    size_t heap_init;
    size_t heap_max;
} VmOptions;

void PrintHelp(void);
void PrintVersionInformation(void);
VmOptions *Parse(int argc, char **argv);
void FreeOptionsIfPossible(VmOptions *opts);
void PrintSummary(VmOptions *options);

extern const VmOptions DEFAULT_OPTIONS;

#endif
