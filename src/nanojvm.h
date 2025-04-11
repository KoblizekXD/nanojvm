#ifndef NANOJVM_H
#define NANOJVM_H

#include <classparse.h>
#include <stddef.h>

typedef struct virtual_machine {
    size_t loaded_classes_count;
    ClassFile **loaded_classes;
} VirtualMachine;

#endif
