#include <classfile.h>
#include <iterators.h>

Iterator CreateFieldIterator(const ClassFile *classfile)
{
    return (Iterator) {
        .index = 0,
        .count = classfile->field_count,
        .classfile = classfile,
        .ptr = (void *) classfile->field_off
    };
}

extern void skip_attributes(void **ptr, uint16_t count);

Field read_field(void **ptr)
{
    Field field = {0};
    field.access_flags = get16(ptr);
    field.name_index = get16(ptr) - 1;
    field.descriptor_index = get16(ptr) - 1;
    field.attributes_count = get16(ptr);
    field.attributes = *ptr;
    skip_attributes(ptr, field.attributes_count);
    return field;
}

Field FieldIteratorNext(Iterator *it)
{
    if (it->index >= it->count) {
        return (Field){0};
    }

    const Field field = read_field(&it->ptr);
    it->index++;
    return field;
}

Field GetField(const ClassFile *classfile, uint16_t index)
{
    if (index >= classfile->field_count) {
        return (Field) {0};
    }
    void *addr = classfile->field_off;

    for (uint16_t i = 0; i < index; i++) {
        skip(addr, 6);
        skip_attributes(&addr, get16(&addr));
    }
    return read_field(&addr);
}