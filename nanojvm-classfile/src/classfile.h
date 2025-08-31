#ifndef NANOJVM_CLASSFILE_CLASSFILE_H
#define NANOJVM_CLASSFILE_CLASSFILE_H

#include <stdint.h>
#include <commons.h>
#include <constants.h>

#define CLASSFILE_META_VALID (1 << 0)
#define CLASSFILE_META_SOURCE_TYPE (1 << 1) // 0 = static, 1 = FILE*
#define CLASSFILE_META_READ_SUCCESS (1 << 2)

typedef struct njvmClassFile {
    uint8_t source_meta;
    void *source;
    uint16_t cpool_count;
    uint16_t if_count;
    void *if_off;
    uint16_t field_count;
    void *field_off;
    uint16_t method_count;
    void *method_off;
    uint16_t attr_count;
    void *attr_off;
} ClassFile;

typedef struct njvmConstantPoolEntry {
    uint16_t tag;
    union njvmConstantPoolEntryDetails {
        struct CONSTANT_Fieldref_Methodref_InterfaceMethodref {
            uint16_t class_index;
            uint16_t name_and_type_index;
        } fieldref_methodref_interfacemethodref;
        struct CONSTANT_String_info {
            uint16_t string_index;
        } string_info;
        struct CONSTANT_Integer_Float_info {
            uint32_t bytes;
        } integer_float_info;
        struct CONSTANT_Long_Double_info {
            uint32_t high_bytes;
            uint32_t low_bytes;
        } long_double_info;
        struct CONSTANT_Class_info {
            uint16_t name_index;
        } class_info;
        struct CONSTANT_NameAndType_info {
            uint16_t name_index;
            uint16_t descriptor_index;
        } name_and_type_info;
        struct CONSTANT_Utf8_info {
            uint16_t length;
            uint8_t *bytes;
        } utf8_info;
        struct CONSTANT_MethodHandle_info {
            uint8_t reference_kind;
            uint16_t reference_index;
        } method_handle_info;
        struct CONSTANT_MethodType_info {
            uint16_t descriptor_index;
        } method_type_info;
        struct CONSTANT_InvokeDynamic_info {
            uint16_t bootstrap_method_attr_index;
            uint16_t name_and_type_index;
        } invoke_dynamic_info;
        struct CONSTANT_Module_info {
            uint16_t name_index;
        } module_info;
        struct CONSTANT_Package_info {
            uint16_t name_index;
        } package_info;
    } info;
} ConstantPoolEntry;

struct njvmAttributeInfo;
typedef struct njvmAttributeInfo AttributeInfo;

typedef struct {
    uint16_t constantvalue_index;
} ConstantValueAttribute;

typedef struct njvmExceptionTableEntry {
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
} __attribute__((packed)) ExceptionTableEntry;

typedef struct njvmCodeAttribute {
    uint16_t max_stack;
    uint16_t max_locals;
    uint32_t code_length;
    uint8_t* code;
    uint16_t exception_table_length;
    ExceptionTableEntry *exception_table;
    uint16_t attributes_count;
    void *attributes;
} CodeAttribute;

typedef struct {} DeprecatedAttribute;

struct njvmAttributeInfo {
    uint16_t attribute_name;
    uint32_t attribute_length;
    uint32_t synth_attribute_type; // Synthetic value for easier distinction - see constants.h for ATTR_* based constants
    union njvmAttributeData {
        ConstantValueAttribute constant_value;
        CodeAttribute code;
        DeprecatedAttribute deprecated;
    } data;
};

typedef struct njvmField {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    void *attributes;
} Field;

typedef struct njvmMethod {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    void *attributes;
} Method;

/**
 * Read a class file offsets from a byte array. This function assumes the source is a static block of memory.
 * The source must remain valid for the lifetime of the ClassFile structure, otherwise undefined behavior may occur.
 *
 * @param source source of class file as byte array
 * @return ClassFile structure
 */
ClassFile ClassFileReadStatic(void *source);

#if FREESTANDING == 0

#include <stdio.h>

/**
 * Read a class file offsets from a FILE pointer. The following calls to functions by this library with the use of
 * returned structure will use the source FILE pointer to read data, so make sure it remains valid for the lifetime of
 * the structure, otherwise undefined behavior may occur.
 *
 * @param source source of class file as FILE pointer
 * @return ClassFile structure
 */
ClassFile ClassFileRead(const FILE *source);

#endif //FREESTANDING

/**
 * Compare a Java UTF-8 string with a C string.
 * Returns 1 if they are equal, 0 otherwise.
 * @note Java strings are not null-terminated, so the null terminator is not considered in the comparison.
 * @param str1 Java UTF-8 string
 * @param str2 C string
 * @return 1 if equal, 0 otherwise
 */
int StringEquals(const struct CONSTANT_Utf8_info *str1, const char *str2);
uint16_t GetAccessFlags(const ClassFile *classfile);
uint16_t GetMajorVersion(const ClassFile *classfile);
uint16_t GetMinorVersion(const ClassFile *classfile);
ConstantPoolEntry GetClassFileName(const ClassFile *classfile);
ConstantPoolEntry GetSuperClassName(const ClassFile *classfile);

#endif //NANOJVM_CLASSFILE_CLASSFILE_H