#include "vmopts.h"
#include <nanojvm.h>
#include <mem/heap.h>
#include <stdio.h>
#include <stdlib.h>

VirtualMachine *vm;

int main(int argc, char **argv)
{
    VmOptions opts = Parse(argc, argv);
    PrintSummary(&opts);
    vm = Initialize(&opts); 
    ClassFile *cf = FindClass(vm, "java/lang/String");
    printf("%s\n", cf->name);
    TearDown(vm);
    return 0;
}
