#ifndef NANOJVM_CORE_UTIL_MEMORY_REGION_LL_H
#define NANOJVM_CORE_UTIL_MEMORY_REGION_LL_H

#include <nanojvm.h>

void RegionAppend(HeapArea *heap, MemoryRegion **base, MemoryRegion *element);
void RegionRemove(HeapArea *heap, MemoryRegion **base, const MemoryRegion *element);
size_t CountRegions(HeapArea *heap, const MemoryRegion *base);
MemoryRegion *RegionFindFit(HeapArea *heap, size_t size);
int AttemptMergeWithAdjacent(HeapArea *heap, MemoryRegion *region);

#endif // NANOJVM_CORE_UTIL_MEMORY_REGION_LL_H