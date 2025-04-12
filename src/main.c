#include <nanojvm.h>
#include <mem/heap.h>
#include <stdlib.h>

volatile VirtualMachine *vm;

int main(int argc, char **argv)
{
    vm = Initialize(NULL);
    // TearDown(vm);
    return 0;
}
