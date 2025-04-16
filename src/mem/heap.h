#ifndef NANOJVM_MEM_HEAP_H
#define NANOJVM_MEM_HEAP_H

#include <stddef.h>
#include <stdint.h>

#define HEAP_MARKED (1 << 0)
#define HEAP_TYPE_0 (1 << 1)
#define HEAP_TYPE_1 (1 << 2)
#define HEAP_ALLOCATED (1 << 7)

// Structure-specific metadata:
#define HEAP_ARRAY_EL_SIZE_0 (1 << 3)
#define HEAP_ARRAY_EL_SIZE_1 (1 << 4)
#define HEAP_ARRAY_FLOATING (1 << 5)

typedef struct heap_region {
    uint8_t metadata;
    size_t size;
    struct heap_region *next;
    uint8_t data[];
} __attribute__((packed)) HeapRegion;

typedef struct heap {
    size_t size;
    HeapRegion *allocated;
    HeapRegion *free;
    void *raw;
} Heap;

Heap *InitializeHeap(size_t size);
HeapRegion *Allocate(Heap *heap, size_t size);
void Free(Heap *heap, HeapRegion *region);
void HeapMap(Heap *heap);
void FreeHeap(Heap *heap);
void ExpandHeap(Heap *heap, size_t new_size);

#endif
