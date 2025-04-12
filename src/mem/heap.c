#include <mem/heap.h>
#include <stdint.h>
#include <util/logging.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

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

Heap *InitializeHeap(size_t size)
{
    Heap *heap = malloc(sizeof(Heap));
    if (heap == NULL) {
        error(LOG_STATEMENT_ALLOC_FAILED, strerror(errno));
        return NULL;
    }
    heap->size = size;
    heap->raw = calloc(1, size);
    if (heap == NULL) {
        error(LOG_STATEMENT_ALLOC_FAILED, strerror(errno));
        return NULL;
    }
    heap->allocated = NULL;
    heap->free = NULL;
    prepare_region(heap, heap->raw, heap->size, 0);
    return heap;
}

HeapRegion *Allocate(Heap *heap, size_t size)
{
    if (heap == NULL || heap->free == NULL) return NULL;
    if (size < sizeof(HeapRegion)) {
        error("Region sized smaller than %d bytes", sizeof(HeapRegion));
        return NULL;
    }

    HeapRegion *big_enough = heap->free;
    HeapRegion *previous = NULL;
    while (big_enough->size < size) {
        previous = big_enough;
        big_enough = big_enough->next;
        if (big_enough == NULL) {
            error("Cannot allocate memory: Insufficent capacity, consider running a garbage collector or defragmentation");
            return NULL;
        }
    }

    HeapRegion *next = prepare_region(heap, (uint8_t*) big_enough + size, big_enough->size - size, 0);
    if (previous != NULL)
        previous->next = next;
    else heap->free = next;
    if (next->size < sizeof(HeapRegion)) {
        warn("Block of memory remaining after split %d|\x1b[1m%d\x1b[0m bytes is smaller than %d bytes, consider defragmentation",
                size, big_enough->size - size, sizeof(HeapRegion));
    }
    big_enough = prepare_region(heap, big_enough, size, 1);

    debug("Allocated region of %d bytes at %p", big_enough->size, (void*) big_enough);
    return big_enough;
}

void Free(Heap *heap, HeapRegion *region)
{
    if (heap->allocated == region && region->next != NULL) {
        heap->allocated = region->next;
        memset(region, 0, region->size);
        return;
    }

    HeapRegion *previous = find_previous(heap->allocated, region);
    assert(previous != NULL);
    previous->next = region->next;
    prepare_region(heap, region, region->size, 0);
}

static void update_list(HeapRegion **list, long diff) 
{
    HeapRegion *prev = NULL;
    HeapRegion *current = *list;
    
    while (current != NULL) {
        if (diff != 0) {
            current = (HeapRegion *)((uint8_t *)current + diff);
        }

        if (prev != NULL) {
            prev->next = current;
        } else {
            *list = current;
        }

        prev = current;
        current = current->next;
    }
}

void ExpandHeap(Heap *heap, size_t new_size)
{
    size_t old_size = heap->size;
    if (heap == NULL || heap->raw == NULL || heap->size > new_size) {
        return;
    }

    uint8_t *old_raw = heap->raw;
    uint8_t *new_raw = realloc(heap->raw, new_size);
    
    if (new_raw == NULL) {
        return;
    }

    long diff = new_raw - old_raw;

    heap->raw = new_raw;
    heap->size = new_size;

    update_list(&heap->allocated, diff);
    update_list(&heap->free, diff);

    prepare_region(heap, (uint8_t*) heap->raw + old_size, new_size - old_size, 0);
}

#define COLOR_FREE "\x1b[42m"
#define COLOR_ALLOC "\x1b[41m"
#define COLOR_RESET "\x1b[0m"
#define BLOCK_SIZE 4

void HeapMap(Heap *heap)
{
    if (heap == NULL || heap->raw == NULL) {
        printf("Heap is not initialized\n");
        return;
    }

    printf("\nHeap Map (Total Size: %zu bytes)\n", heap->size);
    printf("[");

    size_t num_blocks = (heap->size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    char *visual = (char *)calloc(num_blocks + 1, sizeof(char));
    if (visual == NULL) {
        printf("Memory error in HeapMap\n");
        return;
    }

    HeapRegion *current = heap->free;
    while (current != NULL) {
        size_t start = ((uintptr_t)current - (uintptr_t)heap->raw) / BLOCK_SIZE;
        size_t end = start + (current->size + sizeof(HeapRegion) + BLOCK_SIZE - 1) / BLOCK_SIZE;
        
        for (size_t i = start; i < end && i < num_blocks; i++) {
            if (i == start)
                visual[i] = 'S';
            else visual[i] = 'F';  // Free
        }
        current = current->next;
    }

    current = heap->allocated;
    while (current != NULL) {
        size_t start = ((uintptr_t)current - (uintptr_t)heap->raw) / BLOCK_SIZE;
        size_t end = start + (current->size + sizeof(HeapRegion) + BLOCK_SIZE - 1) / BLOCK_SIZE;
        
        for (size_t i = start; i < end && i < num_blocks; i++) {
            if (i == start)
                visual[i] = 'T';
            else visual[i] = 'A';  // Allocated
        }
        current = current->next;
    }

    for (size_t i = 0; i < num_blocks; i++) {
        if (visual[i] == 'F') {
            printf(COLOR_FREE " " COLOR_RESET);
        } else if (visual[i] == 'A') {
            printf(COLOR_ALLOC " " COLOR_RESET);
        } else if (visual[i] == 'S') {
            printf(COLOR_FREE "|" COLOR_RESET);
        } else if (visual[i] == 'T') {
            printf(COLOR_ALLOC "|" COLOR_RESET);
        } else {
            printf(" ");
        }
    }

    printf("]\n");

    printf("\nLegend:\n");
    printf(COLOR_FREE " " COLOR_RESET " Free Block (%d+ bytes)\n", BLOCK_SIZE);
    printf(COLOR_ALLOC " " COLOR_RESET " Allocated Block (%d+ bytes)\n", BLOCK_SIZE);
    printf("Note: Each block represents %d bytes\n", BLOCK_SIZE);

    free(visual);
}

void FreeHeap(Heap *heap)
{
    free(heap->raw);
    free(heap);
}
