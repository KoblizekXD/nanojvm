#ifndef NANOJVM_H
#define NANOJVM_H

#define NANOJVM_REVISION "0.1.0"

#include <classparse.h>

#include <stdint.h>
#include <stddef.h>

#define ERRNO_SEGMENT_TOO_SMALL 0x1
#define ERRNO_HEAP_INIT_FAILURE 0x2
#define ERRNO_HEAP_SEGMENT_TOO_SMALL 0x3
#define ERRNO_HEAP_OOM 0x4

typedef struct cached_string {
    uint64_t hash;
    // ObjectRegion *region;
} __attribute__((aligned(4))) CachedString;

typedef struct thread_frame {
    uint8_t *pc;
    Method *method;
    size_t local_size;
    size_t opstack_size;
    uint8_t data[]; // First contains (4 * local_size) bytes for local variables, then (4 * opstack_size) bytes for operand stack.
} __attribute__((packed)) ThreadFrame;

typedef struct thread {
    size_t frame_count;
    ThreadFrame frames[];
} __attribute__((packed)) Thread;

typedef struct heap Heap;

typedef struct virtual_machine {
    size_t string_pool_count;
    size_t string_pool_size;
    CachedString *strings;
    Heap *heap;
    size_t thread_count;
    size_t thread_pool_size;
    Thread *threads;
    uint16_t errno;
} __attribute__((aligned(4))) FreestandingVirtualMachine;

// Initializes VM and determines the memory segments required for each task. If this operation fails for whatever reason, the errno
// variable will be set in the returning structure.
FreestandingVirtualMachine Initialize(void *memory, size_t total_size);

// Initializes VM in a custom manner, you're free to provide whatever sizing you would like for each of the segments.
// This is preferred if you know what you're doing. If any of segments is incorrectly set(e.g. it is too small), corresponding errno
// will be set.
FreestandingVirtualMachine InitializeEx(
    void *string_pool, size_t string_pool_size, 
    void *heap, size_t heap_size, 
    void *thread_data, size_t thread_data_size,
    void *classfile_pool, size_t classfile_pool_size
);

#endif // NANOJVM_H
