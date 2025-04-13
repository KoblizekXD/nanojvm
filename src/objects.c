#include "mem/exstack.h"
#include <mem/heap.h>
#include <stdlib.h>
#include <util/logging.h>
#include <util/strings.h>
#include <string.h>
#include <nanojvm.h>
#include <classparse.h>
#include <objects.h>

size_t CalculateOffset(VirtualMachine *vm, ClassFile *base, const char *field_name)
{
    size_t sum = 0;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            Field *f = &base->fields[i];
            if (StringEquals(f->name, field_name)) {
                return sum;
            } else sum += f->field_size;
        }
        base = FindClass(vm, base->super_name);
    }
    return sum;
}

size_t CalculateTotalObjectSize(VirtualMachine *vm, ClassFile *base)
{
    size_t sum = 0;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            sum += GetFieldValueSize(&base->fields[i]);
        }
        base = FindClass(vm, base->super_name);
    }
    return sum;
}

ObjectRegion *Instantiate(VirtualMachine *vm, ClassFile *cf)
{
    ObjectRegion *region = (void*) Allocate(vm->heap, sizeof(ObjectRegion) + CalculateTotalObjectSize(vm, cf));
    if (region == NULL) {
        warn("Region is NULL, check logs for possible OutOfMemory");
    } else {
        region->cf = cf;
        region->metadata |= HEAP_TYPE_0 | HEAP_TYPE_1;
    }

    return region;
}

void SetValue(VirtualMachine *vm, ObjectRegion *instance, const char *field_name, void *from, size_t length)
{
    size_t sum = 0;
    ClassFile *base = instance->cf;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            Field *f = &base->fields[i];
            if (StringEquals(f->name, field_name)) {
                if (f->field_size != length) {
                    warn("Field %s not set: Has %d bytes, but requested to copy %d", f->field_size, length);
                    return;
                }
                break;
            } else sum += f->field_size;
        }
        base = FindClass(vm, base->super_name);
    }

    memcpy(instance->data + sum, from, length);
}

void SetValueEx(VirtualMachine *vm, ObjectRegion *instance, const char *field_name, Item *item)
{
    size_t off = CalculateOffset(vm, instance->cf, field_name);
    size_t tocopy = GetItemValueSize(item);

    memcpy(instance->data + off, item->data, tocopy);
}

Item *GetValue(VirtualMachine *vm, ObjectRegion *instance, const char *field_name)
{
    size_t tocopy = 0;
    size_t sum = 0;
    uint8_t metadata = 0;
    ClassFile *base = instance->cf;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            Field *f = &base->fields[i];
            if (StringEquals(f->name, field_name)) {
                tocopy = f->field_size;
                if (strlen(f->descriptor) > 1) {
                    switch (f->descriptor[0]) {
                        case 'Z':
                        case 'B':
                        case 'C':
                            metadata |= STACK_ELEMENT_BYTE;
                            break;
                        case 'S':
                            metadata |= STACK_ELEMENT_SHORT;
                            break;
                        case 'F':
                            metadata |= STACK_ELEMENT_FLOATING | STACK_ELEMENT_INT;
                            break;
                        case 'I':
                            metadata |= STACK_ELEMENT_INT;
                            break;
                        case 'D':
                            metadata |= STACK_ELEMENT_FLOATING | STACK_ELEMENT_LONG;
                            break;
                        case 'J':
                            metadata |= STACK_ELEMENT_LONG;
                            break;
                        default:
                            warn("Undefined descriptor %s", f->descriptor);
                            break;
                    }
                } else metadata = STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG;
                break;
            } else sum += f->field_size;
        }
        base = FindClass(vm, base->super_name);
    }

    Item *item = malloc(sizeof(Item) + tocopy);
    item->metadata = metadata;
    memcpy(item->data, instance->data + sum, tocopy);
    return item;
}

Field *GetFieldRecursively(VirtualMachine *vm, ClassFile *base, const char *field_name)
{
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            Field *f = &base->fields[i];
            if (StringEquals(f->name, field_name)) {
                return f;
            }
        }
        base = FindClass(vm, base->super_name);
    }
    return NULL;
}
