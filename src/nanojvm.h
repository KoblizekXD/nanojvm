#ifndef NANOJVM_H
#define NANOJVM_H

#include <mem/exstack.h>
#include <mem/heap.h>
#include <threads.h>
#include <vmopts.h>
#include <classparse.h>
#include <stddef.h>

#define NANOJVM_REVISION "0.1.0"

typedef struct jdk {
    const char *path;
    uint8_t mode;
    void *handle;
} JDK;

typedef struct vm_frame {
    Method *method;
    uint8_t *pc;
    ExStack *locals;
    ExStack *opstack;
} ThreadFrame;

typedef struct vm_thread {
    thrd_t native_thread;
    size_t frame_count;
    ThreadFrame *frames;
} Thread;

typedef struct virtual_machine {
    const VmOptions *options;
    size_t loaded_classes_count;
    ClassFile **loaded_classes;
    Heap *heap;
    JDK *jdk;
    size_t thread_count;
    Thread *threads;
} VirtualMachine;

typedef struct object_region {
    uint8_t metadata;
    size_t size;
    HeapRegion *next;
    ClassFile *cf;
    uint8_t data[];
} ObjectRegion;

VirtualMachine *Initialize(const VmOptions *options);
void TearDown(VirtualMachine *vm);

/**
 * Attempts to find given Java class in classpath or if it has been already loaded, in memory.
 * If class is not found, NULL is returned.
 */
ClassFile *FindClass(VirtualMachine *vm, const char *name);

/**
 * Returns the thread matching current thread id.
 */
Thread *GetCurrent(VirtualMachine *vm);

/**
 * Executes bytecode instructions present on given method. This assumes the code attribute is present on the method.
 * It is also possible to specify custom local variable stack and operand stack(purely optional, passing NULL will generate custom). Capacity must match code attribute's.
 *
 * @return NULL if code attribute is not present or method returns void. Otherwise returns what method bytecode decides to, but wrapped in Item struct.
 */
Item *ExecuteMethodBytecode(VirtualMachine *vm, Method *method, ExStack *lvars, ExStack *opstack);

JDK *SetupJDK(void);

#endif
