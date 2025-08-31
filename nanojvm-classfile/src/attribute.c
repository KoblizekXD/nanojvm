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

    AttributeInfo attr = {0};
    attr.attribute_name = get16(&it->ptr) - 1;
    attr.attribute_length = get32(&it->ptr);
    const struct CONSTANT_Utf8_info info = GetConstantPoolEntry(it->classfile, attr.attribute_name).info.utf8_info;
    attr.synth_attribute_type = StringHashEx((const char *) info.bytes, info.length);
    switch (attr.synth_attribute_type) {
        case ATTR_CONSTANT_VALUE:
            attr.data.constant_value.constantvalue_index = get16(&it->ptr) - 1;
            break;
        case ATTR_CODE: {
            attr.data.code.max_stack = get16(&it->ptr);
            attr.data.code.max_locals = get16(&it->ptr);
            attr.data.code.code_length = get32(&it->ptr);
            attr.data.code.code = it->ptr;
            skip(it->ptr, attr.data.code.code_length);
            attr.data.code.exception_table_length = get16(&it->ptr);
            attr.data.code.exception_table = (ExceptionTableEntry *) it->ptr;
            skip(it->ptr, attr.data.code.exception_table_length * sizeof(ExceptionTableEntry));
            attr.data.code.attributes_count = get16(&it->ptr);
            attr.data.code.attributes = it->ptr;
            skip_attributes(&it->ptr, attr.data.code.attributes_count);
            break;
        }
        default: break;
    }
    it->index++;
    return attr;
}