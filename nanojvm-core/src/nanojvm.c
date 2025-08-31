#include <nanojvm.h>

VirtualMachine CreateVirtualMachine(
    const njvmMalloc heapAlloc, const njvmFree heapFree, const njvmRealloc heapRealloc,
    const njvmHeapSizing heap_initial_size, const size_t classfile_count, ClassFile *classfiles
) {
    return (VirtualMachine) {
        .classfile_count = classfile_count,
        .classfiles = classfiles,
        .heapAlloc = heapAlloc,
        .heapFree = heapFree,
        .heapRealloc = heapRealloc,
        .heap = (struct njvmHeapConfiguration) {
            .initial_size = heap_initial_size,
            .max_size = UINT64_MAX,
            .current_size = heap_initial_size,
            .heap_conf = HEAP_CONF_ALLOW_GC | HEAP_CONF_ALLOW_EXPAND | HEAP_CONF_ALLOW_SHRINK,
            .memory = NULL
        }
    };
}

void InitializeHeap(VirtualMachine *vm)
{
    vm->heap.memory = vm->heapAlloc(vm->heap.initial_size);
    if (vm->heap.memory == NULL) {
        error("Initial heap allocation of %llu bytes failed, the state of memory is unknown", vm->heap.initial_size);
        return;
    }
    info("Successfully requested %llu bytes of heap memory", vm->heap.initial_size);
}

void DestroyVirtualMachine(const VirtualMachine *vm)
{
    info("Gracefully shutting down VM, freeing heap memory");
    if (vm->heap.memory != NULL) vm->heapFree(vm->heap.memory);
}