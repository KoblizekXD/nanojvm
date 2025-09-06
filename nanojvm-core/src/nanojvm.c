#include <nanojvm.h>

ClassFile *LoadClassFile(const VirtualMachine *vm, const uint8_t *bytes, const size_t length)
{
    for (int i = 0; i < vm->classfile_count; ++i) {
        const ClassFile *cf = &vm->classfiles[i];
        const ConstantPoolEntry entry = GetClassFileName(cf);
        if (entry.tag == 0) continue;
        const struct CONSTANT_Utf8_info name = entry.info.utf8_info;
        if (name.length == length && MemoryCompare(name.bytes, bytes, length) == 0) {
            return (ClassFile *)cf;
        }
    }
    return NULL;
}

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
    vm->heap.memory->used_head = NULL;
    vm->heap.memory->free_head = (MemoryRegion *)vm->heap.memory->memory;
    vm->heap.memory->size = vm->heap.initial_size;
    MemorySet(vm->heap.memory->memory, 0, vm->heap.memory->size);
    vm->heap.memory->free_head->size = vm->heap.initial_size - sizeof(MemoryRegion);
    info("Successfully requested %llu bytes of heap memory", vm->heap.initial_size);
}

void DestroyVirtualMachine(const VirtualMachine *vm)
{
    info("Gracefully shutting down VM, freeing heap memory");
    if (vm->heap.memory != NULL) vm->heapFree(vm->heap.memory);
}