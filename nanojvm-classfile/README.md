# NanoJVM - ClassFile Module

This module provides a minimalistic implementation of the Java ClassFile format, enabling parsing
and obtaining basic information from Java class files. It is designed to be lightweight and freestanding,
making it suitable for environments with limited resources. Everything runs on stack and no heap allocation is required.

## How to use

Once you integrate this module into your project, you can get started with using it.

> [!IMPORTANT]  
> This module uses *Iterator*s extensively to avoid heap allocations. They are quite trivial to use - I recommend checking the `iterators.h` header file for more information.

### Parsing a ClassFile

ClassFile can be parsed in 2 ways - depending on whether you're using freestanding mode or not.

- In freestanding mode, you can use the `ClassFileReadStatic` function, providing a pointer to the class file data in memory.
- In non-freestanding mode, you can also use the `ClassFileRead` function, which takes a `FILE*` pointer to read the class file from a file stream.

### Iterating over ClassFile components

Let's say you want to get some entry from the constant pool. You can create an iterator for the constant pool entries and loop through them.

```c
ClassFile classfile = ...;
Iterator it = CreateConstantPoolIterator(&classfile);
ConstantPoolEntry e = ConstantPoolIteratorNext(&it);
// Or if you know the index:
int index = 5;
e = GetConstantPoolEntry(&classfile, index);
```