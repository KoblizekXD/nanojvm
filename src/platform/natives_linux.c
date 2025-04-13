#if defined(__linux__) || defined(__APPLE__)

#include <platform/natives.h>
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

#endif
