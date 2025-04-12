#include <mem/heap.h>

int main(int argc, char **argv)
{
    Heap *heap = InitializeHeap(1024);
    HeapMap(heap);
    HeapRegion *h1 = Allocate(heap, 128);
    HeapRegion *h2 = Allocate(heap, 128);
    HeapRegion *h3 = Allocate(heap, 128);
    HeapMap(heap);
    Free(heap, h2);
    HeapMap(heap);
    FreeHeap(heap);
    return 0;
}
