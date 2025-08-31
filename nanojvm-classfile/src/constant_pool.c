#include <classfile.h>
#include <iterators.h>

ConstantPoolEntry get_entry(void **ptr)
{
    ConstantPoolEntry entry = {0};
    entry.tag = get8(ptr);
    switch (entry.tag) {
        case CONSTANT_Class:
            entry.info.class_info.name_index = get16(ptr);
            break;
        case CONSTANT_Fieldref:
        case CONSTANT_Methodref:
        case CONSTANT_InterfaceMethodref:
            entry.info.fieldref_methodref_interfacemethodref.class_index = get16(ptr);
            entry.info.fieldref_methodref_interfacemethodref.name_and_type_index = get16(ptr);
            break;
        case CONSTANT_String:
            entry.info.string_info.string_index = get16(ptr) - 1;
            break;
        case CONSTANT_Integer:
        case CONSTANT_Float:
            entry.info.integer_float_info.bytes = get32(ptr);
            break;
        case CONSTANT_Long:
        case CONSTANT_Double:
            entry.info.long_double_info.high_bytes = get32(ptr);
            entry.info.long_double_info.low_bytes = get32(ptr);
            break;
        case CONSTANT_NameAndType:
            entry.info.name_and_type_info.name_index = get16(ptr) - 1;
            entry.info.name_and_type_info.descriptor_index = get16(ptr) - 1;
            break;
        case CONSTANT_Utf8:
            entry.info.utf8_info.length = get16(ptr);
            entry.info.utf8_info.bytes = *ptr;
            skip(*ptr, entry.info.utf8_info.length);
            break;
        case CONSTANT_MethodHandle:
            entry.info.method_handle_info.reference_kind = get8(ptr);
            entry.info.method_handle_info.reference_index = get16(ptr) - 1;
            break;
        case CONSTANT_MethodType:
            entry.info.method_type_info.descriptor_index = get16(ptr) - 1;
            break;
        case CONSTANT_Dynamic:
        case CONSTANT_InvokeDynamic:
            entry.info.invoke_dynamic_info.bootstrap_method_attr_index = get16(ptr) - 1;
            entry.info.invoke_dynamic_info.name_and_type_index = get16(ptr) - 1;
            break;
        case CONSTANT_Package:
        case CONSTANT_Module:
            entry.info.package_info.name_index = get16(ptr) - 1;
            break;
        default: return (ConstantPoolEntry){0};
    }
    return entry;
}

ConstantPoolEntry ConstantPoolIteratorNext(Iterator *it)
{
    if (it->index >= it->count) {
        return (ConstantPoolEntry){.tag = 0};
    }

    const ConstantPoolEntry entry = get_entry(&it->ptr);
    it->index++;
    return entry;
}

Iterator CreateConstantPoolIterator(const ClassFile *classfile)
{
    return (Iterator){
        .index = 0,
        .count = classfile->cpool_count,
        .ptr = offset(classfile->source, 10),
        .classfile = classfile
    };
}

ConstantPoolEntry GetConstantPoolEntry(const ClassFile *classfile, const uint16_t index)
{
    if (index >= classfile->cpool_count) {
        return (ConstantPoolEntry){0};
    }

    void *addr = offset(classfile->source, 10);
    for (uint16_t i = 0; i < index; i++) {
        const uint8_t tag = get8(&addr);
        switch (tag) {
            case CONSTANT_Class:
            case CONSTANT_String:
            case CONSTANT_MethodType:
            case CONSTANT_Package:
            case CONSTANT_Module:
                skip(addr, 2);
                break;
            case CONSTANT_MethodHandle:
                skip(addr, 3);
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref:
            case CONSTANT_Dynamic:
            case CONSTANT_InvokeDynamic:
            case CONSTANT_Integer:
            case CONSTANT_Float:
            case CONSTANT_NameAndType:
                skip(addr, 4);
                break;
            case CONSTANT_Long:
            case CONSTANT_Double:
                skip(addr, 8);
                i++;
                break;
            case CONSTANT_Utf8: {
                const uint16_t size = get16(&addr);
                skip(addr, size);
                break;
            }
            default: return (ConstantPoolEntry){0};
        }
    }
    return get_entry(&addr);
}