#ifndef NANOJVM_H
#define NANOJVM_H

#include <vmopts.h>
#include <classparse.h>
#include <stddef.h>

#define NANOJVM_REVISION "0.1.0"

typedef struct virtual_machine {
    VmOptions *options;
    size_t loaded_classes_count;
    ClassFile **loaded_classes;
} VirtualMachine;

#endif
