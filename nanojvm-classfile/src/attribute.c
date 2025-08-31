#include <classfile.h>
#include <iterators.h>

struct kv_pair {
    uint32_t hash;
    int value;
} __attribute__((__packed__));

static const struct kv_pair kv_pairs[] = {
    { 0x00b20b4c,ATTR_CONSTANT_VALUE },
    { 0x7c83b260,ATTR_CODE },
    { 0x65528041,ATTR_STACK_MAP_TABLE },
    { 0xb0bb9c47,ATTR_EXCEPTIONS },
    { 0xab595e6f,ATTR_INNER_CLASSES },
    { 0x0c60f188,ATTR_ENCLOSING_METHOD },
    { 0x8b42a240,ATTR_SYNTHETIC },
    { 0x9dd84ab7,ATTR_SIGNATURE },
    { 0x36cfd5d6,ATTR_SOURCE_FILE },
    { 0x35d544ba,ATTR_SOURCE_DEBUG_EXTENSION },
    { 0xbe9b7afe,ATTR_LINE_NUMBER_TABLE },
    { 0x90d6b3de,ATTR_LOCAL_VARIABLE_TABLE },
    { 0x0d33dd80,ATTR_LOCAL_VARIABLE_TYPE_TABLE },
    { 0xd4a0ded6,ATTR_DEPRECATED },
    { 0x6bfed7c5,ATTR_RUNTIME_VISIBLE_ANNOTATIONS },
    { 0xc0fccbbc,ATTR_RUNTIME_INVISIBLE_ANNOTATIONS },
    { 0xc30663a6,ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS },
    { 0xf297bd7d,ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS },
    { 0xaf5a8b67,ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS },
    { 0x798fa2de,ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS },
    { 0xde491da5,ATTR_ANNOTATION_DEFAULT },
    { 0x2aaa2277,ATTR_BOOTSTRAP_METHODS },
    { 0x0aa5c1da,ATTR_METHOD_PARAMETERS },
    { 0xc38db30b,ATTR_MODULE },
    { 0xf303020a,ATTR_MODULE_PACKAGES },
    { 0xc8aea4e6,ATTR_MODULE_MAIN_CLASS },
    { 0xeb19dc3d,ATTR_NEST_HOST },
    { 0xa258b38a,ATTR_NEST_MEMBERS },
    { 0xce81e404,ATTR_RECORD },
    { 0xf43544ab,ATTR_PERMITTED_SUBCLASSES }
};

// Fast function to match a string to an attribute type using a KV pair array and a simple hash function(djb2).
uint16_t match(const char *array, const size_t size)
{
    const size_t hash = StringHashEx(array, size);
    for (size_t i = 0; i < sizeof(kv_pairs) / sizeof(kv_pairs[0]); i++) {
        if (kv_pairs[i].hash == hash) {
            return kv_pairs[i].value;
        }
    }
    return ATTR_UNKNOWN;
}

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
    attr.synth_attribute_type = match((const char *) info.bytes, info.length);
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