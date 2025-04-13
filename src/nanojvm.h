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

#define FrameCeiling(THREAD) THREAD->frames[THREAD->frame_count - 1]

typedef struct virtual_machine {
    VmOptions *options;
    size_t loaded_classes_count;
    ClassFile **loaded_classes;
    Heap *heap;
    JDK *jdk;
    size_t thread_count;
    Thread *threads;
    size_t natives_count;
    void **natives;
} VirtualMachine;

// Initializes the VM, looks for JDK installation, classpath & options etc. Doesn't execute anything on its own.
VirtualMachine *Initialize(VmOptions *options);

// Performs a cleanup, destroying any trace of the VM.
void TearDown(VirtualMachine *vm);

/**
 * Attempts to find given Java class in classpath or if it has been already loaded, in memory.
 * If class is not found, NULL is returned.
 */
ClassFile *FindClass(VirtualMachine *vm, const char *name);

/**
 * Forcibly loads class file into the VM's memory.
 * This is useful when not using default classpath and using NanoJVM as a library.
 * Please be aware that this function won't check if a class is already present, so it is possible
 * to put duplicate classes inside.
 */
void ForceLoad(VirtualMachine *vm, ClassFile *cf);

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

/**
 * Executes bytecode instructions present on given method assuming code attribute exists.
 * This method will automatically pickup current thread's frame stack and push/pop to it accordingly.
 * It will also take parameters from the top frame's operand stack.
 *
 * @return NULL if code attribute is not present or mehtod returns void. Otherwise returns what method bytecode would.
 */
Item *InvokeMethod(VirtualMachine *vm, Method *method);

/**
 * Looks for a valid JDK installation, this can be later used to resolve basic Java classes.
 * Technically not required, but is recommended.
 */
JDK *SetupJDK(void);

#endif
