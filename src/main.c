#include <mem/heap.h>

int main(int argc, char **argv)
{
    Heap *heap = InitializeHeap(1024);
    HeapRegion *h1 = Allocate(heap, 128);
    HeapRegion *h2 = Allocate(heap, 128);
    HeapRegion *h3 = Allocate(heap, 128);
    Free(heap, h2);
    HeapMap(heap);
    ExpandHeap(heap, 2048);
    HeapMap(heap);
    FreeHeap(heap);
    return 0;
}
