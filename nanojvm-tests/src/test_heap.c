#include <greatest.h>
#include <stdio.h>
#include <nanojvm.h>
#include <util/memory_region_ll.h>

VirtualMachine vm;

static void Setup(void *data)
{
    vm = CreateVirtualMachine(malloc, free, realloc, 1024, 0, NULL);
    InitializeHeap(&vm);
}

void visualize_heap(const HeapArea *heap) {
    printf("Heap Visualization:\n");
    MemoryRegion *current = heap->free_head;
    printf("Free List:\n");
    while (current != NULL) {
        printf("  Region at %lu: size=%zu, used=%s\n", ToVirtual(heap, current), current->size, (current->metadata & HEAP_MEMORY_USED) ? "yes" : "no");
        if (current->next == 0) break;
        current = FromVirtual(heap, current->next);
    }
    current = heap->used_head;
    printf("Used List:\n");
    while (current != NULL) {
        printf("  Region at %lu: size=%zu, used=%s\n", ToVirtual(heap, current), current->size, (current->metadata & HEAP_MEMORY_USED) ? "yes" : "no");
        if (current->next == 0) break;
        current = FromVirtual(heap, current->next);
    }
}

static void TearDown(void *data)
{
    DestroyVirtualMachine(&vm);
}

TEST TestHeapInitialization(void)
{
    ASSERT(vm.heap.memory != NULL);
    ASSERT(vm.heap.max_size == UINT64_MAX);
    ASSERT(vm.heap.memory->size == 1024);
    ASSERT(vm.heap.memory->free_head->size == 1024 - sizeof(MemoryRegion));
    PASS();
}

TEST TestSimpleHeapAllocation(void)
{
    const VirtualAddress addr = Malloc(&vm, 128);
    ASSERT(addr != 0);
    const MemoryRegion *reg = FromVirtual(vm.heap.memory, addr);
    ASSERT(reg->metadata & HEAP_MEMORY_USED);
    ASSERT(reg->size == 128);
    ASSERT(CountRegions(vm.heap.memory, vm.heap.memory->used_head) == 1);
    ASSERT(CountRegions(vm.heap.memory, vm.heap.memory->free_head) == 1);
    Free(&vm, addr);
    ASSERT(CountRegions(vm.heap.memory, vm.heap.memory->used_head) == 0);
    ASSERT(CountRegions(vm.heap.memory, vm.heap.memory->free_head) == 1);
    PASS();
}

TEST TestMultipleHeapAllocations(void)
{
    const VirtualAddress addr = Malloc(&vm, 128);
    ASSERT(addr != 0);
    const VirtualAddress addr2 = Malloc(&vm, 256);
    visualize_heap(vm.heap.memory);
    Free(&vm, addr);
    Free(&vm, addr2);
    visualize_heap(vm.heap.memory);
    ASSERT(CountRegions(vm.heap.memory, vm.heap.memory->used_head) == 0);
    ASSERT(CountRegions(vm.heap.memory, vm.heap.memory->free_head) == 1);
    PASS();
}

GREATEST_SUITE(heap_suite)
{
    SET_SETUP(Setup, NULL);
    SET_TEARDOWN(TearDown, NULL);
    RUN_TEST(TestHeapInitialization);
    RUN_TEST(TestSimpleHeapAllocation);
    RUN_TEST(TestMultipleHeapAllocations);
}