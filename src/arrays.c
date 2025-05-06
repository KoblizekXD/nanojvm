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
