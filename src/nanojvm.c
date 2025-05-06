#include <classparse.h>
#include <nanojvm.h>
#include <memory/heap.h>
#include <memory/utils.h>

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
        .thread_count = 1,
        .threads = thread_data,
        .strings = string_pool,
        .heap = heap,
        .cf_count = 0,
        .cf_pool_size = classfile_pool_size,
        .cf_table = classfile_pool
    };
    vm.heap->size = heap_size - sizeof(Heap);
    vm.threads->frame_count = 0;
    return vm;
}

ClassFile *FindClass(FreestandingVirtualMachine *vm, const char *name)
{
    size_t offset = 0;
    uint8_t *addr = vm->cf_table;
    for (size_t i = 0; i < vm->cf_count; i++) {
        uint32_t size;
        memcpy(&size, addr + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        ClassFile *cf = (void*) (addr + offset);
        if (strcmp(cf->name, name) == 0)
            return cf;
    }
    return NULL;
}

ClassFile *LoadClass(FreestandingVirtualMachine *vm, void *ptr)
{
    size_t required = CalculateRequiredSize(ptr);
    uint8_t *addr = vm->cf_table;
    size_t offset = 0;
    for (size_t i = 0; i < vm->cf_count; i++) {
        uint32_t size;
        memcpy(&size, addr + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t); 
    }
    memcpy(addr + offset, &required, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    if (vm->cf_pool_size < required + offset) {
        vm->errno = ERRNO_CF_OOM;
        return NULL;
    }
    ClassFile *cf = ReadFrom(ptr, addr + offset, required);
    vm->cf_count++;
    return cf;
}
