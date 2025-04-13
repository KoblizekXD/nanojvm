#include <vmopts.h>
#include <nanojvm.h>
#include <mem/heap.h>
#include <stdio.h>

VirtualMachine *vm;

int main(int argc, char **argv)
{
    VmOptions opts = Parse(argc, argv);
    PrintSummary(&opts);
    vm = Initialize(&opts); 
    TearDown(vm);
    return 0;
}
