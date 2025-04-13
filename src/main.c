#include "util/logging.h"
#include <vmopts.h>
#include <nanojvm.h>
#include <mem/heap.h>

VirtualMachine *vm;

int main(int argc, char **argv)
{
    VmOptions opts = Parse(argc, argv);
    PrintSummary(&opts);
    vm = Initialize(&opts);

    ClassFile *cf = LoadExternal(vm, opts.classpath[opts.classpath_len - 1]);

    if (!cf) {
        error("File '%s' must be a valid classfile!", opts.classpath[opts.classpath_len - 1]);
        TearDown(vm);
        return 1;
    }

    TearDown(vm);
    return 0;
}
