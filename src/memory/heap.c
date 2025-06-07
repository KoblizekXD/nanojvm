#include <nanojvm.h>
#include <memory/heap.h>
#include <memory/utils.h>

HeapRegion *prepare_region(Heap *h, void *base, size_t size, uint8_t as_allocated)
{
    HeapRegion *reg = base;
    memset(base, 0, size);
    reg->next = NULL;
    reg->size = size;
    reg->metadata = 0;

    HeapRegion *last;
    if (as_allocated) {
        reg->metadata |= HEAP_ALLOCATED;
        last = h->allocated;
    } else last = h->free;

    if (last == NULL) {
        if (as_allocated)
            h->allocated = reg;
        else h->free = reg;
        return reg;
    }
    while (last != NULL && last->next != NULL) {
        last = last->next;
    }
    last->next = reg;

    return reg;
}

// Finds an entry preceding WHAT in linked list starting with HEAD
HeapRegion *find_previous(HeapRegion *head, HeapRegion *what)
{
    if (!head || !what || head == what) {
        return NULL;
    }

    HeapRegion *current = head;
    while (current->next && current->next != what) {
        current = current->next;
    }

    return current->next == what ? current : NULL;
}

void InitializeHeap(FreestandingVirtualMachine *vm)
{
    vm->heap->allocated = NULL;
    vm->heap->free = NULL;
    if (prepare_region(vm->heap, vm->heap->raw, vm->heap->size, 0) == NULL)
        vm->errno = ERRNO_HEAP_INIT_FAILURE;
}

HeapRegion *Allocate(FreestandingVirtualMachine *vm, size_t size)
{
    Heap *heap = vm->heap;
    // size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1); // Align to system alignment, we ideally don't want
    if (heap == NULL || heap->free == NULL) return NULL;
    if (size < sizeof(HeapRegion)) {
        vm->errno = ERRNO_SEGMENT_TOO_SMALL;
        return NULL;
    }

    HeapRegion *big_enough = heap->free;
    HeapRegion *previous = NULL;
    while (big_enough->size < size) {
        previous = big_enough;
        big_enough = big_enough->next;
        if (big_enough == NULL) {
            vm->errno = ERRNO_HEAP_OOM;
            return NULL;
        }
    }

    HeapRegion *next = prepare_region(heap, (uint8_t*) big_enough + size, big_enough->size - size, 0);
    if (previous != NULL)
        previous->next = next;
    else heap->free = next;
    /*
    if (next->size < sizeof(HeapRegion)) {
        warn("Block of memory remaining after split %d|\x1b[1m%d\x1b[0m bytes is smaller than %d bytes, consider defragmentation",
                size, big_enough->size - size, sizeof(HeapRegion));
    }*/
    big_enough = prepare_region(heap, big_enough, size, 1);

    return big_enough;
}

void Free(FreestandingVirtualMachine *vm, HeapRegion *region)
{
    Heap *heap = vm->heap;
    if (heap->allocated == region && region->next != NULL) {
        heap->allocated = region->next;
        memset(region, 0, region->size);
        return;
    }

    HeapRegion *previous = find_previous(heap->allocated, region);
    previous->next = region->next;
    prepare_region(heap, region, region->size, 0);
}
