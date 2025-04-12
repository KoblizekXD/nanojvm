#ifndef NANOJVM_H
#define NANOJVM_H

#include <mem/heap.h>
#include <vmopts.h>
#include <classparse.h>
#include <stddef.h>

#define NANOJVM_REVISION "0.1.0"

typedef struct jdk {
    const char *path;
    uint8_t mode;
    void *handle;
} JDK;

typedef struct virtual_machine {
    const VmOptions *options;
    size_t loaded_classes_count;
    ClassFile **loaded_classes;
    Heap *heap;
    JDK *jdk;
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

JDK *SetupJDK(void);

#endif
