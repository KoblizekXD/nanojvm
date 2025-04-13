#include <util/strings.h>
#include <string.h>
#include <nanojvm.h>
#include <classparse.h>
#include <objects.h>

size_t CalculateOffset(VirtualMachine *vm, ClassFile *base, const char *field_name)
{
    size_t sum = 0;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            Field *f = &base->fields[i];
            if (StringEquals(f->name, field_name)) {
                return sum;
            } else sum += GetFieldValueSize(f);
        }
        base = FindClass(vm, base->super_name);
    }
    return sum;
}

size_t CalculateTotalObjectSize(VirtualMachine *vm, ClassFile *base)
{
    size_t sum = 0;
    while (base != NULL) {
        for (size_t i = 0; i < base->field_count; i++) {
            sum += GetFieldValueSize(&base->fields[i]);
        }
        base = FindClass(vm, base->super_name);
    }
    return sum;
}
