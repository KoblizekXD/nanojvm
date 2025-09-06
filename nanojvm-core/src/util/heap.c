#include <nanojvm.h>
#include <util/memory_region_ll.h>

VirtualAddress Malloc(const VirtualMachine *vm, const size_t size)
{
    debug("Heap memory request of %zu bytes(%zu bytes incl. metadata)", size, size + sizeof(MemoryRegion));
    if (size == 0) {
        debug("Heap memory size is zero");
        return 0;
    }
    MemoryRegion *reg = RegionFindFit(vm->heap.memory, size);
    if (reg == NULL) {
        error("Out of memory: failed to allocate %zu bytes", size);
        return 0;
    }
    MemoryRegion *next = (void*) reg->data + size;
    next->metadata = 0;
    next->size = reg->size - size - sizeof(MemoryRegion);
    reg->metadata |= HEAP_MEMORY_USED;
    reg->size = size;

    RegionAppend(vm->heap.memory, &vm->heap.memory->used_head, reg);
    RegionRemove(vm->heap.memory, &vm->heap.memory->free_head, reg);
    RegionAppend(vm->heap.memory, &vm->heap.memory->free_head, next);
    return ToVirtual(vm->heap.memory, reg);
}

void Free(const VirtualMachine *vm, const VirtualAddress addr)
{
    if (addr == 0) return;
    MemoryRegion *reg = FromVirtual(vm->heap.memory, addr);
    if (!(reg->metadata & HEAP_MEMORY_USED)) {
        error("Double free detected at address %p", (void *)addr);
        return;
    }
    RegionRemove(vm->heap.memory, &vm->heap.memory->used_head, reg);
    reg->metadata &= ~HEAP_MEMORY_USED;
    RegionAppend(vm->heap.memory, &vm->heap.memory->free_head, reg);
    AttemptMergeWithAdjacent(vm->heap.memory, vm->heap.memory->free_head);
}