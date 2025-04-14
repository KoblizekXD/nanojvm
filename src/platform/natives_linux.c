#if defined(__linux__) || defined(__APPLE__)

#include <platform/natives.h>
#include <mem/memutils.h>
#include <util/logging.h>
#include <dlfcn.h>

void *OpenLibrary(const char *path)
{
    void *lib = dlopen(path, RTLD_LAZY);
    if (!lib) {
        warn("Failed to open native library: %s", dlerror());
    }

    return lib;
}

void CloseLibrary(void *handle)
{
    dlclose(handle);
}

NativeFunction FindNativeFunction(void *library, const char *name)
{
    void *symbol = dlsym(library, name);
    return (NativeFunction) symbol;
}

#include <unistd.h>
#include <sys/resource.h>
#include <malloc.h>

MemoryInformation GetMemoryState(void)
{
    MemoryInformation info = {0, 0};
    
    FILE* statm = fopen("/proc/self/statm", "r");
    if (statm) {
        unsigned long size, resident, shared, text, lib, data, dt;
        if (fscanf(statm, "%lu %lu %lu %lu %lu %lu %lu", 
                  &size, &resident, &shared, &text, &lib, &data, &dt) == 7) {
            long page_size = sysconf(_SC_PAGESIZE);  // Typically 4096 bytes
            info.total_usage = resident * page_size;
        }
        fclose(statm);
    }
    
    struct mallinfo2 mi = mallinfo2();
    info.heap_allocated = mi.uordblks;  // Total allocated space
    
    if (info.total_usage == 0) {
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            info.total_usage = usage.ru_maxrss * 1024;
        }
    }
    
    return info;
}

#endif
