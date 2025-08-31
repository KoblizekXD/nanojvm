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

Method read_method(void **ptr)
{
    Method method;
    method.access_flags = get16(ptr);
    method.name_index = get16(ptr) - 1;
    method.descriptor_index = get16(ptr) - 1;
    method.attributes_count = get16(ptr);
    method.attributes = *ptr;
    skip_attributes(ptr, method.attributes_count);
    return method;
}

Method MethodIteratorNext(Iterator *it)
{
    if (it->index >= it->count) {
        return (Method){0};
    }

    const Method method = read_method(&it->ptr);
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
    return read_method(&addr);
}