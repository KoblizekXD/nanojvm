#include <nanojvm.h>

// Simple first-fit memory region locator for malloc
MemoryRegion *find_fit(HeapArea *heap, const size_t size)
{
    MemoryRegion *current = heap->free_head;
    while (current != NULL) {
        if (!(current->metadata & HEAP_MEMORY_USED) && (current->size + sizeof(MemoryRegion)) >= size) {
            return current;
        }
        if (current->next == 0) break;
        current = FromVirtual(heap, current->next);
    }
    return NULL;
}

// Append an element to the end of a virtual memory linked list
void append_ll(HeapArea *heap, MemoryRegion **base, MemoryRegion *element)
{
    if (*base == NULL) {
        *base = element;
        return;
    }
    MemoryRegion *current = *base;
    while (current->next != 0) {
        current = FromVirtual(heap, current->next);
    }
    current->next = ToVirtual(heap, element);
}

void remove_used(HeapArea *heap, const MemoryRegion *reg)
{
    if (heap->used_head == reg) {
        heap->used_head = (reg->next == 0) ? NULL : (MemoryRegion *)((uint8_t *)heap + reg->next);
        return;
    }
    MemoryRegion *current = heap->used_head;
    while (current != NULL && current->next != 0) {
        MemoryRegion *next = (MemoryRegion *)((uint8_t *)heap + current->next);
        if (next == reg) {
            current->next = next->next;
            return;
        }
        current = next;
    }
}

VirtualAddress Malloc(const VirtualMachine *vm, const size_t size)
{
    MemoryRegion *reg = find_fit(vm->heap.memory, size);
    if (reg == NULL) {
        error("Out of memory: failed to allocate %zu bytes", size);
        return 0;
    }
    MemoryRegion *next = (void*) reg->data + size;
    next->metadata = 0;
    next->size = reg->size - size - sizeof(MemoryRegion);
    next->next = reg->next;
    reg->metadata |= HEAP_MEMORY_USED;
    reg->size = size;
    reg->next = 0;
    append_ll(vm->heap.memory, &vm->heap.memory->used_head, reg);
    append_ll(vm->heap.memory, &vm->heap.memory->free_head, reg);
    return ToVirtual(vm->heap.memory, reg->data);
}

void Free(const VirtualMachine *vm, const VirtualAddress addr)
{
    if (addr == 0) return;
    MemoryRegion *reg = (MemoryRegion *) FromVirtual(vm->heap.memory, addr) - 1;
    if (!(reg->metadata & HEAP_MEMORY_USED)) {
        error("Double free detected at address %p", (void *)addr);
        return;
    }
    reg->metadata &= ~HEAP_MEMORY_USED;
    remove_used(vm->heap.memory, reg);
}