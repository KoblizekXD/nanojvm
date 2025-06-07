#include <nanojvm.h>
#include <memory/utils.h>
#include <memory/heap.h>

size_t CalculateOffset(FreestandingVirtualMachine *vm, ClassFile *base, const char *field_name)
{
    size_t sum = 0;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            Field *f = &base->fields[i];
            if (strcmp(f->name, field_name) == 0) {
                return sum;
            } else sum += f->field_size;
        }
        base = FindClass(vm, base->super_name);
    }
    return sum;
}

size_t CalculateTotalObjectSize(FreestandingVirtualMachine *vm, ClassFile *base)
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


ObjectRegion *Instantiate(FreestandingVirtualMachine *vm, ClassFile *cf)
{
    ObjectRegion *region = (void*) Allocate(vm, sizeof(ObjectRegion) + CalculateTotalObjectSize(vm, cf));
    if (region != NULL) {
        region->cf = cf;
        region->metadata |= HEAP_TYPE_0 | HEAP_TYPE_1;
    }
    return region;
}

int SetArrayValue(HeapRegion *instance, size_t index, void *from)
{
    if ((instance->metadata & (HEAP_TYPE_0 | HEAP_TYPE_1)) == (HEAP_TYPE_0 | HEAP_TYPE_1)) return 0;
    size_t elsize = GetArrayElementSize(instance);
    void *off = NULL;
    if (instance->metadata & HEAP_TYPE_0) {
        off = (uint8_t*) instance + sizeof(PrimitiveArrayRegion) + (index * elsize);
    } else {
        off = (uint8_t*) instance + sizeof(ObjectArrayRegion) + (index * elsize);
    }
    memcpy(off, from, elsize);
    return 1;
}

size_t GetArrayElementSize(HeapRegion *region)
{
    uint8_t flags = region->metadata;
    uint8_t size_flags = flags & (HEAP_ARRAY_EL_SIZE_0 | HEAP_ARRAY_EL_SIZE_1);
    
    switch (size_flags) {
        case 0:
            return 1;
        case HEAP_ARRAY_EL_SIZE_0:
            return 2;
        case HEAP_ARRAY_EL_SIZE_1:
            return 4;
        case (HEAP_ARRAY_EL_SIZE_0 | HEAP_ARRAY_EL_SIZE_1):
            return 8;
        default:
            return 0;
    }
}

void SetValue(FreestandingVirtualMachine *vm, ObjectRegion *instance, const char *field_name, void *from, size_t length)
{
    size_t sum = 0;
    ClassFile *base = instance->cf;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            Field *f = &base->fields[i];
            if (strcmp(f->name, field_name) == 0) {
                if (f->field_size != length) {
                    return;
                }
                break;
            } else sum += f->field_size;
        }
        base = FindClass(vm, base->super_name);
    }

    memcpy(instance->data + sum, from, length);
}


