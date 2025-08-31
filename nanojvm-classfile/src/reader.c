#include <classfile.h>

void skip_cpool(void **ptr, const uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        const uint8_t tag = get8(ptr);
        switch (tag) {
            case 7: // CONSTANT_Class
            case 8: // CONSTANT_String
            case 16: // CONSTANT_MethodType
                skip(*ptr, 2);
                break;
            case 3: // CONSTANT_Integer
            case 4: // CONSTANT_Float
            case 9: // CONSTANT_Fieldref
            case 10: // CONSTANT_Methodref
            case 11: // CONSTANT_InterfaceMethodref
            case 12: // CONSTANT_NameAndType
            case 18: // CONSTANT_InvokeDynamic
                skip(*ptr, 4);
                break;
            case 5: // CONSTANT_Long
            case 6: // CONSTANT_Double
                skip(*ptr, 8);
                i++; // takes up two entries in the constant pool
                break;
            case 15: // CONSTANT_MethodHandle
                skip(*ptr, 3);
                break;
            case 1: { // CONSTANT_Utf8
                const uint16_t length = get16(ptr);
                skip(*ptr, length);
                break;
            }
            default:
                return;
        }
    }
}

void skip_attributes(void **ptr, const uint16_t count) {
    for (uint16_t i = 0; i < count; i++) {
        skip(*ptr, 2); // attribute_name_index
        const uint32_t attribute_length = get32(ptr);
        skip(*ptr, attribute_length);
    }
}

void skip_fields(void **ptr, const uint16_t count) {
    for (uint16_t i = 0; i < count; i++) {
        skip(*ptr, 6); // access_flags, name_index, descriptor_index
        const uint16_t attributes_count = get16(ptr);
        skip_attributes(ptr, attributes_count);
    }
}

void skip_methods(void **ptr, const uint16_t count) {
    for (uint16_t i = 0; i < count; i++) {
        skip(*ptr, 6); // access_flags, name_index, descriptor_index
        const uint16_t attributes_count = get16(ptr);
        skip_attributes(ptr, attributes_count);
    }
}

ClassFile ClassFileReadStatic(void *source)
{
    ClassFile classfile = {0};
    void *ptr = source;
    if (ptr == NULL) return classfile;

    const uint32_t magic = get32(&ptr);
    if (magic != 0xCAFEBABE) {
        return classfile;
    }
    classfile.source_meta |= CLASSFILE_META_VALID; // mark as valid
    classfile.source = ptr - 4;
    classfile.source_meta |= CLASSFILE_META_SOURCE_TYPE; // static source

    skip(ptr, 4);
    classfile.cpool_count = get16(&ptr) - 1;
    skip_cpool(&ptr, classfile.cpool_count);
    skip(ptr, 6);
    classfile.if_count = get16(&ptr);
    classfile.if_off = ptr;
    skip(ptr, classfile.if_count * 2);
    classfile.field_count = get16(&ptr);
    classfile.field_off = ptr;
    skip_fields(&ptr, classfile.field_count);
    classfile.method_count = get16(&ptr);
    classfile.method_off = ptr;
    skip_methods(&ptr, classfile.method_count);
    classfile.attr_count = get16(&ptr);
    classfile.attr_off = ptr;
    skip_attributes(&ptr, classfile.attr_count);
    classfile.source_meta |= CLASSFILE_META_READ_SUCCESS;
    return classfile;
}

#if FREESTANDING == 0

ClassFile ClassFileRead(const FILE *source)
{
    error("Implementation missing, please read the whole file and use ClassFileReadStatic instead.\n");
    return (ClassFile) {0};
}

#endif
