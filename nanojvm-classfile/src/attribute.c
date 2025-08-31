#include <classfile.h>
#include <iterators.h>

Iterator CreateAttributeIterator(const ClassFile *classfile, const void *source, const uint8_t meta)
{
    switch (meta) {
        case ITERATOR_ATTRIBUTE_SOURCE_CLASS: {
            return (Iterator){
                .index = 0,
                .count = classfile->attr_count,
                .ptr = classfile->attr_off,
                .classfile = classfile
            };
        }
        case ITERATOR_ATTRIBUTE_SOURCE_CODE: {
            return (Iterator){
                .index = 0,
                .count = ((CodeAttribute *) source)->attributes_count,
                .ptr = ((CodeAttribute *) source)->attributes,
                .classfile = classfile
            };
        }
        case ITERATOR_ATTRIBUTE_SOURCE_FIELD: {
            return (Iterator){
                .index = 0,
                .count = ((Field *) source)->attributes_count,
                .ptr = ((Field *) source)->attributes,
                .classfile = classfile
            };
        }
        case ITERATOR_ATTRIBUTE_SOURCE_METHOD: {
            return (Iterator){
                .index = 0,
                .count = ((Method *) source)->attributes_count,
                .ptr = ((Method *) source)->attributes,
                .classfile = classfile
            };
        }
        default: return (Iterator){0};
    }
}

extern void skip_attributes(void **ptr, uint16_t count);

AttributeInfo AttributeIteratorNext(Iterator *it)
{
    if (it->index >= it->count) {
        return (AttributeInfo){0};
    }

    const AttributeInfo attr = ReadAttributeAt(it->classfile, &it->ptr);
    it->index++;
    return attr;
}

AttributeInfo skip_attributes_until(const ClassFile *cf, void *ptr, const uint16_t count, const uint32_t synth_attribute_type)
{
    for (uint16_t i = 0; i < count; i++) {
        const uint16_t name_index = get16(&ptr) - 1;
        const uint32_t attribute_length = get32(&ptr);
        const struct CONSTANT_Utf8_info info = GetConstantPoolEntry(NULL, name_index).info.utf8_info;
        const uint32_t current_synth_attribute_type = StringHashEx((const char *) info.bytes, info.length);
        if (current_synth_attribute_type == synth_attribute_type) {
            skip(ptr, -6);
            return ReadAttributeAt(cf, &ptr);
        }
        skip(ptr, attribute_length);
    }
    return (AttributeInfo){0};
}

AttributeInfo GetAttributeInfo(const ClassFile *classfile, void *attr_ptr, const uint16_t attr_count, const uint32_t synth_attribute_type)
{
    return skip_attributes_until(classfile, attr_ptr, attr_count, synth_attribute_type);
}

AttributeInfo ReadAttributeAt(const ClassFile *classfile, void **ptr)
{
    AttributeInfo attr = {0};
    attr.attribute_name = get16(ptr) - 1;
    attr.attribute_length = get32(ptr);
    const struct CONSTANT_Utf8_info info = GetConstantPoolEntry(classfile, attr.attribute_name).info.utf8_info;
    attr.synth_attribute_type = StringHashEx((const char *) info.bytes, info.length);
    switch (attr.synth_attribute_type) {
        case ATTR_CONSTANT_VALUE:
            attr.data.constant_value.constantvalue_index = get16(ptr) - 1;
            break;
        case ATTR_CODE: {
            attr.data.code.max_stack = get16(ptr);
            attr.data.code.max_locals = get16(ptr);
            attr.data.code.code_length = get32(ptr);
            attr.data.code.code = *ptr;
            skip(*ptr, attr.data.code.code_length);
            attr.data.code.exception_table_length = get16(ptr);
            attr.data.code.exception_table = (ExceptionTableEntry *) *ptr;
            skip(*ptr, attr.data.code.exception_table_length * sizeof(ExceptionTableEntry));
            attr.data.code.attributes_count = get16(ptr);
            attr.data.code.attributes = *ptr;
            skip_attributes(ptr, attr.data.code.attributes_count);
            break;
        }
        default: break;
    }
    return attr;
}