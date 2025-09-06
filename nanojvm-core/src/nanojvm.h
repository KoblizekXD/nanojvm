#ifndef NANOJVM_CORE_NANOJVM_H
#define NANOJVM_CORE_NANOJVM_H

#include <commons.h>
#include <classfile.h>
#include <util/array_stack.h>

#ifndef NANOJVM_OVERRIDE_HEAP_SIZING
typedef uint64_t njvmHeapSizing;
#else
typedef NANOJVM_OVERRIDE_HEAP_SIZING njvmHeapSizing;
#endif

#define HEAP_CONF_ALLOW_GC (1 << 0)
#define HEAP_CONF_ALLOW_SHRINK (1 << 1)
#define HEAP_CONF_ALLOW_EXPAND (1 << 2)

/**
 * NanoJVM uses virtual addresses to reference memory locations within its managed heap.
 * It represents an offset from the start of the heap memory block, this removes the additional
 * overhead of relocating raw pointers when the heap is resized or moved.
 */
typedef uintptr_t VirtualAddress;

#define HEAP_MEMORY_USED (1 << 0)

/**
 * Structure representing a memory area in the heap. Each memory area contains
 * metadata about the allocation, its size, a pointer to the next memory area,
 * and the actual data block.
 */
typedef struct njvmMemoryRegion {
    uint8_t metadata; // metadata flags (e.g., allocation status)
    size_t size; // size of the data block
    VirtualAddress next; // virtual address of the next memory region
    uint8_t data[]; // flexible array member for the data block
} __attribute__((packed)) MemoryRegion;

typedef struct njvmHeapArea {
    njvmHeapSizing size; // total size of the heap area(including struct data)
    MemoryRegion *free_head; // pointer to the head of the free list
    MemoryRegion *used_head; // pointer to the head of the used list
    MemoryRegion memory[];
} __attribute__((packed)) HeapArea;

typedef struct njvmVirtualMachine VirtualMachine;

/**
 * Function, which allocates a block of memory in VM's heap memory area.
 * @param vm the virtual machine instance
 * @param size the size to attempt to allocate
 * @return a virtual address representing the allocated memory block, or 0 on failure
 */
VirtualAddress Malloc(const VirtualMachine *vm, size_t size);

/**
 * Function, which frees a previously allocated block of memory in VM's heap memory area.
 * The VM will throw an error if the address is invalid or was not dynamically allocated.
 * @param vm the virtual machine instance
 * @param addr the virtual address to free
 */
void Free(const VirtualMachine *vm, VirtualAddress addr);

/**
 * Function, which resizes a previously allocated block of memory in VM's heap memory area.
 * @param heap the heap area
 * @param ptr the raw pointer to convert
 */
#define ToVirtual(heap, ptr) ((VirtualAddress)((uint8_t *)ptr - (uint8_t *)heap))

/**
 * Converts a virtual address to a raw pointer within the heap memory area.
 * @param heap the heap area
 * @param addr the virtual address to convert
 */
#define FromVirtual(heap, addr) ((void *)((uint8_t *)heap + addr))

struct njvmVirtualMachine {
    njvmMalloc heapAlloc;
    njvmFree heapFree;
    njvmRealloc heapRealloc;
    struct njvmHeapConfiguration {
        njvmHeapSizing initial_size;
        njvmHeapSizing max_size;
        uint8_t heap_conf;
        HeapArea *memory;
    } heap;
    size_t classfile_count;
    ClassFile *classfiles;
};

#define RESULT_OK 0
#define RESULT_EXIT 1
#define RESULT_EXCEPTION 2

/**
 * Structure representing the result of a method or frame execution.
 * The value field can hold different types based on the execution outcome:
 * - For normal returns, it holds the NULL
 * - For exceptions, it holds a pointer to the exception object
 * - For exit signals, it holds the exit code as an integer
 */
typedef struct njvmExecutionResult {
    int state;
    uintptr_t value;
} ExecutionResult;

/**
 * Creates a new VirtualMachine instance with provided parameters. This function will
 * not allocate memory for the heap; use InitializeHeap() for that purpose.
 * @param heapAlloc Heap allocation function, used for all heap-related memory requests
 * @param heapFree  Heap free function, used for all heap-related memory releases
 * @param heapRealloc Heap reallocation function, used for resizing heap memory blocks
 * @param heap_initial_size Initial size of the heap in bytes
 * @param classfile_count Number of class files provided
 * @param classfiles Array of class files to be loaded into the VM
 * @return A VirtualMachine instance initialized with the provided parameters
 */
VirtualMachine CreateVirtualMachine(
    njvmMalloc heapAlloc,
    njvmFree heapFree,
    njvmRealloc heapRealloc,
    njvmHeapSizing heap_initial_size,
    size_t classfile_count,
    ClassFile *classfiles
);

/**
 * Initializes the heap for the given VirtualMachine instance. This function allocates
 * memory for the heap based on the initial size specified during VM creation.
 * @param vm Pointer to the VirtualMachine instance to initialize the heap for
 */
void InitializeHeap(VirtualMachine *vm);

/**
 * Begins execution of the provided method in the current thread context. This function
 * sets up the necessary thread frame and starts interpreting bytecode from the method.
 * @param vm the VirtualMachine instance to execute the method in
 * @param classfile the class file containing the method to execute
 * @param method the method to begin execution from
 * @return status code returned from execution (usually 0 for success, non-zero for error)
 */
int ThreadCurrentExecute(const VirtualMachine *vm, const ClassFile *classfile, Method *method);

/**
 * Destroys the given VirtualMachine instance, freeing all associated resources.
 * @param vm Pointer to the VirtualMachine instance to be destroyed
 */
void DestroyVirtualMachine(const VirtualMachine *vm);

#endif // NANOJVM_CORE_NANOJVM_H