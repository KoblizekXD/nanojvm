#include <classfile.h>
#include <iterators.h>

Iterator CreateMethodIterator(const ClassFile *classfile)
{
    return (Iterator) {
        .index = 0,
        .count = classfile->method_count,
        .classfile = classfile,
        .ptr = (void *) classfile->method_off
    };
}

extern void skip_attributes(void **ptr, uint16_t count);

void *find_code_attribute(const ClassFile *cf, void *ptr, const uint16_t attributes_count)
{
    for (uint16_t i = 0; i < attributes_count; i++) {
        const uint16_t name_index = get16(&ptr) - 1;
        const uint32_t length = get32(&ptr);
        const struct CONSTANT_Utf8_info temp = GetConstantPoolEntry(cf, name_index).info.utf8_info;
        if (StringHashEx((const char *) temp.bytes, temp.length) == ATTR_CODE) {
            return offset(ptr, -6);
        }
        skip(ptr, length);
    }
    return NULL;
}

Method read_method(const ClassFile *cf, void **ptr)
{
    Method method;
    method.access_flags = get16(ptr);
    method.name_index = get16(ptr) - 1;
    method.descriptor_index = get16(ptr) - 1;
    method.attributes_count = get16(ptr);
    method.attributes = *ptr;
    method.code_attr_off = find_code_attribute(cf, *ptr, method.attributes_count);
    skip_attributes(ptr, method.attributes_count);
    return method;
}

Method MethodIteratorNext(Iterator *it)
{
    if (it->index >= it->count) {
        return (Method){0};
    }

    const Method method = read_method(it->classfile, &it->ptr);
    it->index++;
    return method;
}

Method GetMethod(const ClassFile *classfile, const uint16_t index)
{
    if (index >= classfile->cpool_count) {
        return (Method) {0};
    }
    void *addr = classfile->method_off;

    for (uint16_t i = 0; i < index; i++) {
        skip(addr, 6);
        skip_attributes(&addr, get16(&addr));
    }
    return read_method(classfile, &addr);
}

CodeAttribute GetCodeAttribute(const ClassFile *cf, const Method *method)
{
    void *off = method->code_attr_off;
    return ReadAttributeAt(cf, &off).data.code;
}