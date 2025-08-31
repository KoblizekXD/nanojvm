#include <classfile.h>
#include <iterators.h>

int StringEquals(const struct CONSTANT_Utf8_info *str1, const char *str2) {
    if (str1->length != StringLength(str2)) {
        return 0;
    }
    for (uint16_t i = 0; i < str1->length; i++) {
        if (str1->bytes[i] != (uint8_t)str2[i]) {
            return 0;
        }
    }
    return 1;
}

uint16_t GetAccessFlags(const ClassFile *classfile)
{
    void *addr = offset(classfile->if_off, -8);
    return get16(&addr);
}

uint16_t GetMajorVersion(const ClassFile *classfile)
{
    void *addr = offset(classfile->source, 6);
    return get16(&addr);
}

uint16_t GetMinorVersion(const ClassFile *classfile)
{
    void *addr = offset(classfile->source, 4);
    return get16(&addr);
}

ConstantPoolEntry GetClassFileName(const ClassFile *classfile)
{
    void *addr = offset(classfile->if_off, -6);
    return GetConstantPoolEntry(classfile, GetConstantPoolEntry(classfile, get16(&addr) - 1).info.class_info.name_index - 1);
}

ConstantPoolEntry GetSuperClassName(const ClassFile *classfile)
{
    void *addr = offset(classfile->if_off, -4);
    return GetConstantPoolEntry(classfile, GetConstantPoolEntry(classfile, get16(&addr) - 1).info.class_info.name_index - 1);
}

int IteratorHasNext(const Iterator *it)
{
    return it->index < it->count;
}