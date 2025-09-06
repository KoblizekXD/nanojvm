#include <util/memory_region_ll.h>

void RegionAppend(HeapArea *heap, MemoryRegion **base, MemoryRegion *element)
{
    if (*base == NULL) {
        *base = element;
        element->next = 0;
        return;
    }

    MemoryRegion *current = *base;
    const VirtualAddress virtual_element = ToVirtual(heap, element);

    while (current->next != 0 && current->next < virtual_element) {
        current = FromVirtual(heap, current->next);
    }

    element->next = current->next;
    current->next = virtual_element;
}

void RegionRemove(HeapArea *heap, MemoryRegion **base, const MemoryRegion *element)
{
    if (*base == element) {
        *base = (element->next == 0) ? NULL : FromVirtual(heap, element->next);
        return;
    }

    MemoryRegion *current = *base;
    while (current != NULL && current->next != 0) {
        MemoryRegion *next = FromVirtual(heap, current->next);
        if (next == element) {
            current->next = next->next;
            return;
        }
        current = next;
    }
}

size_t CountRegions(HeapArea *heap, const MemoryRegion *base)
{
    size_t count = 0;
    const MemoryRegion *current = base;
    while (current != NULL) {
        count++;
        if (current->next == 0) break;
        current = FromVirtual(heap, current->next);
    }
    return count;
}

MemoryRegion * RegionFindFit(HeapArea *heap, size_t size)
{
}
