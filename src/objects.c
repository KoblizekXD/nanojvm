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

ObjectRegion *InstantiateString(VirtualMachine *vm, const char *data)
{
    uint64_t hash = ComputeHash(data);
    if (vm->string_pool) {
        for (size_t i = 0; i < vm->string_count; i++) {
            if (vm->string_pool[i].hash == hash)
                return vm->string_pool[i].instance;
        }
    } 
    ObjectRegion *reg = Instantiate(vm, FindClass(vm, "java/lang/String"));
    uint8_t coder = 0;
    SetValue(vm, reg, "coder", &coder, 1);
    vm->string_count++;
    vm->string_pool = realloc(vm->string_pool, sizeof(struct string_entry) * vm->string_count);
    vm->string_pool[vm->string_count - 1].hash = hash;
    vm->string_pool[vm->string_count - 1].instance = reg;
    return reg;
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

int Extends(VirtualMachine *vm, ClassFile *what, ClassFile *who)
{
    while (what != NULL) {
        if (what == who || StringEquals(what->name, who->name)) return 1;
        what = FindClass(vm, what->super_name);
    }
    return 0;
}
