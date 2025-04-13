#ifndef NANOJVM_OBJECTS_H
#define NANOJVM_OBJECTS_H

#include <mem/exstack.h>
#include <nanojvm.h>
#include <classparse.h>
#include <mem/heap.h>

#include <stddef.h>
#include <stdint.h>

typedef struct object_region {
    uint8_t metadata;
    size_t size;
    HeapRegion *next;
    ClassFile *cf;
    uint8_t data[];
} ObjectRegion;

/**
 * Create a new instance of a given class. The region is allocated in the heap memory of the virtual machine.
 */
ObjectRegion *Instantiate(VirtualMachine *vm, ClassFile *cf);

/**
 * Sets a value for a field with given name. The field will be checked if length
 * matches the type of field. If this check passes LENGTH bytes are copied from FROM address.
 */
void SetValue(VirtualMachine *vm, ObjectRegion *instance, const char *field_name, void *from, size_t length);

/**
 * Sets a value for a field with given name. The field will be checked if length
 * matches the type of field. If this succeeds, data from ITEM are copied to the field position.
 */
void SetValueEx(VirtualMachine *vm, ObjectRegion *instance, const char *field_name, Item *item);

/**
 * Returns a value of the field wrapped in Item structure. If it is unitialized, default value will be used.
 */
Item *GetValue(VirtualMachine *vm, ObjectRegion *instance, const char *field_name);

/**
 * Calculates the offset of a field_name in the instance object, where base is a most bottom class file to start at.
 * VM instance is required here to look at superclasses recursively.
 */
size_t CalculateOffset(VirtualMachine *vm, ClassFile *base, const char *field_name);

/**
 * Calculates the total size of fields that is required to allocate base's instance.
 */
size_t CalculateTotalObjectSize(VirtualMachine *vm, ClassFile *base);

// Recursive variant of GetFieldByName, will traverse through superclasses too
Field *GetFieldRecursively(VirtualMachine *vm, ClassFile *base, const char *field_name);

#endif
