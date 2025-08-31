#ifndef NANOJVM_CLASSFILE_ITERATORS_H
#define NANOJVM_CLASSFILE_ITERATORS_H

#include <classfile.h>

typedef struct njvmIterator {
    size_t index;
    size_t count;
    const ClassFile *classfile;
    void *ptr;
} Iterator;

Iterator CreateConstantPoolIterator(const ClassFile *classfile);
ConstantPoolEntry ConstantPoolIteratorNext(Iterator *it);
ConstantPoolEntry GetConstantPoolEntry(const ClassFile *classfile, uint16_t index);

#define ITERATOR_ATTRIBUTE_SOURCE_CLASS 0
#define ITERATOR_ATTRIBUTE_SOURCE_FIELD 1
#define ITERATOR_ATTRIBUTE_SOURCE_METHOD 2
#define ITERATOR_ATTRIBUTE_SOURCE_CODE 3

/**
 * Create an attribute iterator from a given source. The source can be one of:
 * - Class attributes (meta = ITERATOR_ATTRIBUTE_SOURCE_CLASS, source = ClassFile*)
 * - Field attributes (meta = ITERATOR_ATTRIBUTE_SOURCE_FIELD, source = Field*)
 * - Method attributes (meta = ITERATOR_ATTRIBUTE_SOURCE_METHOD, source = Method*)
 * - Code attributes (meta = ITERATOR_ATTRIBUTE_SOURCE_CODE, source = CodeAttribute*)
 *
 * @param classfile the classfile to iterate attributes from
 * @param source the pointer to the source of attributes, the type is specified by meta parameter
 * @param meta the source type, one of ITERATOR_ATTRIBUTE_SOURCE_* constants
 * @return an attribute iterator
 */
Iterator CreateAttributeIterator(const ClassFile *classfile, const void *source, uint8_t meta);
AttributeInfo AttributeIteratorNext(Iterator *it);
AttributeInfo GetAttributeInfo(const void *source, uint8_t meta, uint16_t index);

Iterator CreateFieldIterator(const ClassFile *classfile);
Field FieldIteratorNext(Iterator *it);
Field GetField(const ClassFile *classfile, uint16_t index);

Iterator CreateMethodIterator(const ClassFile *classfile);
Method MethodIteratorNext(Iterator *it);
Method GetMethod(const ClassFile *classfile, uint16_t index);

int IteratorHasNext(const Iterator *it);

#endif // NANOJVM_CLASSFILE_ITERATORS_H