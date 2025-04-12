#include <nanojvm.h>
#include <stdio.h>
#include <vmopts.h>

void PrintHelp(void)
{
    printf("Usage: nanojvm [options] <main> [args...]\n\nArguments appended after the main class will be passed as the arguments to the main class.\n\nOptions:\n");
}

void PrintVersionInformation(void)
{
#ifdef __linux__
    const char *platform = "linux";
#elif defined(_WIN32)
    const char *platform = "win32";
#elif defined(__APPLE__)
    const char *platform = "macos";
#elif defined(__EMSCRIPTEN__)
    const char *platform = "wasm";
#else
    const char *platform = "undefined";
#endif
    printf("NanoJVM version %s\nBuilt at: %s %s for %s-%d-bit\n", NANOJVM_REVISION, __DATE__, __TIME__, platform, (int)(8 * sizeof(void*)));
}

const VmOptions DEFAULT_OPTIONS = (VmOptions) {
    .argc = 0,
    .argv = NULL,
    .classpath = NULL,
    .classpath_len = 0,
    .flags = 0,
    .heap_init = DEFAULT_HEAP_INITIAL,
    .heap_max = DEFAULT_HEAP_MAX
};
