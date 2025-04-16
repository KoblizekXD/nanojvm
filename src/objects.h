#ifndef NANOJVM_OBJECTS_H
#define NANOJVM_OBJECTS_H

#include <mem/exstack.h>
#include <nanojvm.h>
#include <classparse.h>
#include <mem/heap.h>

#include <stddef.h>
#include <stdint.h>

// ObjectRegion for storing object data
typedef struct object_region {
    uint8_t metadata;
    size_t size;
    HeapRegion *next;
    ClassFile *cf;
    uint8_t data[];
} __attribute__((packed)) ObjectRegion;

// Struct representing byte, char, boolean, short, int and long primitive arrays.
typedef struct primitive_array_region {
    uint8_t metadata;
    size_t size;
    HeapRegion *next;
    uint8_t data[];
} __attribute__((packed)) PrimitiveArrayRegion;

// Structure representing arrays of Object arrays. Shares the same structure as ObjectRegion, but is managed differently.
typedef ObjectRegion ObjectArrayRegion;

/**
 * Create a new instance of a given class. The region is allocated in the heap memory of the virtual machine.
 */
ObjectRegion *Instantiate(VirtualMachine *vm, ClassFile *cf);

/**
 * Create a new instance of an array of given length. The array will be allocated in heap memory of the virtual machine. 
 */
PrimitiveArrayRegion *InstantiateArray(VirtualMachine *vm, uint8_t flags, size_t length);

/**
 * Create a new instance of an array of objects of given class. The array will be allocated in heap memory of the VM.
 */
ObjectArrayRegion *InstantiateObjectArray(VirtualMachine *vm, ClassFile *cf, size_t length);

/**
 * Sets a value in the array at given index. The size of the value will be computed automatically and copied from the
 * provided pointer. The function accepts both PrimitiveArrayRegion and ObjectArrayRegion. Passing anything else will result in returning 0.
 * Otherwise returns 1.
 */
int SetArrayValue(HeapRegion *instance, size_t index, void *from);

/**
 * Gets a value present on given array index wrapped as a Item. If given instance is not
 * an array, NULL will be returned. Getting value out of bounds of the array will result in crash.
 */
Item *GetArrayValue(HeapRegion *instance, size_t index);

/**
 * Returns a size of a 1 array element. It will be either 8, 4, 2 or 1.
 */
size_t GetArrayElementSize(HeapRegion *region);

/**
 * Returns a length of given array in heap memory.
 */
size_t GetArrayLength(HeapRegion *reg);

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

/**
 * Create a new instance of a String class with bytes set to provided data.
 */
ObjectRegion *InstantiateString(VirtualMachine *vm, const char *data);

// Recursive variant of GetFieldByName, will traverse through superclasses too
Field *GetFieldRecursively(VirtualMachine *vm, ClassFile *base, const char *field_name);

// Checks if WHAT extends or is WHO
int Extends(VirtualMachine *vm, ClassFile *what, ClassFile *who);

#endif
