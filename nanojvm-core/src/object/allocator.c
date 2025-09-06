#include <nanojvm.h>
#include <iterators.h>

size_t get_field_size(const struct CONSTANT_Utf8_info descriptor)
{
    const uint8_t c = descriptor.bytes[0];
    switch (c) {
        case 'B':
        case 'C':
        case 'Z':
            return 1;
        case 'D':
        case 'J':
            return 8;
        case 'F':
        case 'I':
            return 4;
        case 'L':
        case '[':
            return sizeof(VirtualAddress);
        case 'S':
            return 2;
        default: {
            error("Invalid field descriptor: %c", c);
            return 0;
        }
    }
}

size_t get_classfile_member_size(const VirtualMachine *vm, const ClassFile *classfile) {
    size_t total = 0;
    Iterator it = CreateFieldIterator(classfile);
    while (IteratorHasNext(&it)) {
        const Field field = FieldIteratorNext(&it);
        total += get_field_size(GetConstantPoolEntry(classfile, field.descriptor_index).info.utf8_info);
    }
    const ConstantPoolEntry super = GetSuperClassName(classfile);
    if (super.tag == 0) return total;
    return total + get_classfile_member_size(vm, LoadClassFile(vm, super.info.utf8_info.bytes, super.info.utf8_info.length));
}

ObjectRegion *AllocateObject(const VirtualMachine *vm, ClassFile *classfile)
{
    ObjectRegion *region = (void*) Malloc(vm, get_classfile_member_size(vm, classfile) + (sizeof(ObjectRegion) - sizeof(MemoryRegion)));
    if (!region) {
        error("Failed to allocate object");
        return NULL;
    }
    region->classfile = classfile;
    region->metadata |= HEAP_MEMORY_TYPE_1;
    return region;
}

ObjectArrayRegion *AllocateObjectArray(const VirtualMachine *vm, ClassFile *classfile, const size_t length)
{
    ObjectArrayRegion *region = (void*) Malloc(
        vm,
        (get_classfile_member_size(vm, classfile) * length) + (sizeof(ObjectArrayRegion) - sizeof(MemoryRegion))
    );
    if (!region) {
        error("Failed to allocate object array");
        return NULL;
    }
    region->classfile = classfile;
    region->metadata |= HEAP_MEMORY_TYPE_1 | HEAP_MEMORY_TYPE_2;
    return region;
}

PrimitiveArrayRegion *AllocatePrimitiveArray(const VirtualMachine *vm, const uint8_t element_size, const size_t length)
{
    PrimitiveArrayRegion *region = (void*) Malloc(vm, (element_size * length) + (sizeof(PrimitiveArrayRegion) - sizeof(MemoryRegion)));
    if (!region) {
        error("Failed to allocate primitive array");
        return NULL;
    }
    region->metadata |= HEAP_MEMORY_TYPE_2;
    return region;
}