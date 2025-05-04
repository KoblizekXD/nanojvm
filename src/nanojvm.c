#include <nanojvm.h>
#include <memory/heap.h>

#ifndef NANOJVM_CLASSFILE_POOL_SIZE
#define NANOJVM_CLASSFILE_POOL_SIZE 2048
#endif // NANOJVM_CLASSFILE_POOL_SIZE

FreestandingVirtualMachine Initialize(void *memory, size_t total_size)
{
    return (FreestandingVirtualMachine) {
        .errno = ERRNO_SEGMENT_TOO_SMALL
    };
}

FreestandingVirtualMachine InitializeEx(
    void *string_pool, size_t string_pool_size, 
    void *heap, size_t heap_size, 
    void *thread_data, size_t thread_data_size,
    void *classfile_pool, size_t classfile_pool_size
) {
    if (string_pool_size < sizeof(CachedString)
            || heap_size < sizeof(Heap)
            || thread_data_size < (sizeof(Thread) + sizeof(ThreadFrame))
            || classfile_pool_size < NANOJVM_CLASSFILE_POOL_SIZE)
        return (FreestandingVirtualMachine) { .errno = ERRNO_SEGMENT_TOO_SMALL };

    FreestandingVirtualMachine vm = (FreestandingVirtualMachine) {
        .errno = 0,
        .string_pool_count = 0,
        .string_pool_size = string_pool_size,
        .thread_pool_size = thread_data_size,
        .thread_count = 0,
        .threads = thread_data,
        .strings = string_pool,
        .heap = heap
    };
    vm.heap->size = heap_size - sizeof(Heap);
    return vm;
}
