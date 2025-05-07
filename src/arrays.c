#include <nanojvm.h>
#include <memory/heap.h>
#include <memory/utils.h>

void set_heap_array_el_size(uint8_t* flags, uint8_t value)
{
    *flags &= ~(HEAP_ARRAY_EL_SIZE_0 | HEAP_ARRAY_EL_SIZE_1);

    switch (value) {
        case 1:
            break;
        case 2:
            *flags |= HEAP_ARRAY_EL_SIZE_0; // 01
            break;
        case 4:
            *flags |= HEAP_ARRAY_EL_SIZE_1; // 10
            break;
        case 8:
            *flags |= (HEAP_ARRAY_EL_SIZE_0 | HEAP_ARRAY_EL_SIZE_1); // 11
            break;
        default:
            break;
    }
}

PrimitiveArrayRegion *InstantiateArray(FreestandingVirtualMachine *vm, uint8_t flags, size_t length)
{
    PrimitiveArrayRegion *region = (void*) Allocate(vm, sizeof(PrimitiveArrayRegion) + (flags * length));
    if (region != NULL) {
        region->metadata |= HEAP_TYPE_0;
        set_heap_array_el_size(&region->metadata, flags);
    }

    return region;
}

ObjectArrayRegion *InstantiateObjectArray(FreestandingVirtualMachine *vm, ClassFile *cf, size_t length)
{
    ObjectArrayRegion *region = (void*) Allocate(vm, sizeof(ObjectArrayRegion) + (8 * length));
    if (region != NULL) {
        region->cf = cf;
        region->metadata |= HEAP_ARRAY_EL_SIZE_1 | HEAP_ARRAY_EL_SIZE_0 | HEAP_TYPE_1;
    }

    return region;
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

size_t GetArrayLength(HeapRegion *instance)
{
    size_t per = GetArrayElementSize(instance);
    if (instance->metadata & HEAP_TYPE_0) {
        return (instance->size - sizeof(PrimitiveArrayRegion)) / per;
    } else {
        return (instance->size - sizeof(ObjectArrayRegion)) / per; 
    }
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
