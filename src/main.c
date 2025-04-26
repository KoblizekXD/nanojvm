#ifndef __freestanding__

#include <classparse.h>
#include <stdlib.h>
#include <util/logging.h>
#include <vmopts.h>
#include <nanojvm.h>
#include <mem/heap.h>

VirtualMachine *vm;

void safe_exit(void)
{
    debug("External exit call was initiated...");
    TearDown(vm);
}

int main(int argc, char **argv)
{
    VmOptions *opts= Parse(argc, argv);
    PrintSummary(opts);
    vm = Initialize(opts);
    atexit(safe_exit);

    ClassFile *cf = LoadExternal(vm, opts->classpath[opts->classpath_len - 1]);
    
    if (!cf) {
        error("File '%s' must be a valid classfile", opts->classpath[opts->classpath_len - 1]);
        TearDown(vm);
        return 1;
    }
    Method *m = GetMethodByNameAndDescriptor(cf, "main", "([Ljava/lang/String;)V");

    if (!m) {
        error("File '%s' must have a valid Java entrypoint", cf->name);
        TearDown(vm);
        return 1;
    }

    InvokeMethod(vm, m);

    // TearDown(vm);
    return 0;
}

#else

#warn "Building NanoJVM in freestanding model, as an app, this is not supported. Please set BUILD_AS_LIB=1"

int main(int argc, char *argv[])
{
    return 1;
}

#endif
