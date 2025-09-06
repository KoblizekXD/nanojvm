#include <greatest.h>
#include <stdio.h>
#include <classfile.h>
#include <iterators.h>

uint8_t *buffer;
ClassFile cf;

static void Setup(void *data)
{
    FILE *test_fp = fopen("java/Test1.class", "rb");
    fseek(test_fp, 0, SEEK_END);
    const long size = ftell(test_fp);
    rewind(test_fp);
    buffer = malloc(size);
    fread(buffer, 1, size, test_fp);
    cf = ClassFileReadStatic(buffer);
    fclose(test_fp);
}

static void TearDown(void *data)
{
    free(buffer);
}

TEST TestValidateClassFileRead(void)
{
    ASSERT(cf.source_meta & (CLASSFILE_META_VALID | CLASSFILE_META_READ_SUCCESS));
    PASS();
}

TEST TestGetClassFileBasicInformation(void)
{
    ASSERT(GetMajorVersion(&cf) == 68);
    ASSERT(GetMinorVersion(&cf) == 0);
    ConstantPoolEntry entry = GetClassFileName(&cf);
    ASSERT(StringEquals(&entry.info.utf8_info, "Test1"));
    entry = GetSuperClassName(&cf);
    ASSERT(StringEquals(&entry.info.utf8_info, "java/lang/Object"));
    ASSERT(GetAccessFlags(&cf) & (ACC_PUBLIC | ACC_SUPER));
    PASS();
}

TEST TestClassFileConstantPoolIterator(void)
{
    Iterator it = CreateConstantPoolIterator(&cf);
    int i = 0;
    while (IteratorHasNext(&it)) {
        ConstantPoolEntry e = ConstantPoolIteratorNext(&it);
        if (i == 3) {
            ASSERT(e.tag == CONSTANT_Utf8);
            ASSERT(StringEquals(&e.info.utf8_info, "java/lang/Object"));
        }
        i++;
    }
    ASSERT(i == cf.cpool_count);
    const ConstantPoolEntry e = GetConstantPoolEntry(&cf, 3);
    ASSERT(e.tag == CONSTANT_Utf8);
    ASSERT(StringEquals(&e.info.utf8_info, "java/lang/Object"));
    PASS();
}

TEST TestMethodInformation(void)
{
    ASSERT(cf.method_count == 3);
    Iterator it = CreateMethodIterator(&cf);
    int i = 0;
    while (IteratorHasNext(&it)) {
        MethodIteratorNext(&it);
        i++;
    }
    ASSERT(i == cf.method_count);
    const Method m = GetMethod(&cf, 1);
    ConstantPoolEntry entry = GetConstantPoolEntry(&cf, m.name_index);
    ASSERT(entry.tag == CONSTANT_Utf8 && StringEquals(&entry.info.utf8_info, "main"));
    entry = GetConstantPoolEntry(&cf, m.descriptor_index);
    ASSERT(entry.tag == CONSTANT_Utf8 && StringEquals(&entry.info.utf8_info, "([Ljava/lang/String;)V"));
    PASS();
}

TEST TestMethodAttributeInformation(void)
{
    const Method m = GetMethod(&cf, 1);
    ASSERT(m.attributes_count == 1);
    Iterator it = CreateAttributeIterator(&cf, &m, ITERATOR_ATTRIBUTE_SOURCE_METHOD);
    int i = 0;
    while (IteratorHasNext(&it)) {
        AttributeInfo inf = AttributeIteratorNext(&it);
        ASSERT(inf.synth_attribute_type == ATTR_CODE);
        ASSERT(inf.data.code.code_length == 9);
        Iterator inner = CreateAttributeIterator(&cf, &inf.data.code, ITERATOR_ATTRIBUTE_SOURCE_CODE);
        int j = 0;
        while (IteratorHasNext(&inner)) {
            const AttributeInfo inner_inf = AttributeIteratorNext(&inner);
            ASSERT(inner_inf.synth_attribute_type == ATTR_LINE_NUMBER_TABLE);
            j++;
        }
        ASSERT(j == 1);
        i++;
    }
    ASSERT(i == m.attributes_count);
    PASS();
}

TEST TestMethodQuickCodeLookup(void)
{
    const Method m = GetMethod(&cf, 1);
    ASSERT(m.code_attr_off != NULL);
    const CodeAttribute code_attr = GetCodeAttribute(&cf, &m);
    ASSERT(code_attr.code_length == 9);
    PASS();
}

TEST TestFieldInformation(void)
{
    ASSERT(cf.field_count == 2);
    const Field f = GetField(&cf, 1);
    ConstantPoolEntry entry = GetConstantPoolEntry(&cf, f.name_index);
    ASSERT(entry.tag == CONSTANT_Utf8 && StringEquals(&entry.info.utf8_info, "x"));
    entry = GetConstantPoolEntry(&cf, f.descriptor_index);
    ASSERT(entry.tag == CONSTANT_Utf8 && StringEquals(&entry.info.utf8_info, "I"));
    ASSERT(f.access_flags & (ACC_PRIVATE | ACC_STATIC));
    PASS();
}

TEST TestFieldAttributeInformation(void)
{
    const Field f = GetField(&cf, 0);
    ASSERT(f.attributes_count == 1);
    Iterator it = CreateAttributeIterator(&cf, &f, ITERATOR_ATTRIBUTE_SOURCE_FIELD);
    int i = 0;
    while (IteratorHasNext(&it)) {
        AttributeInfo inf = AttributeIteratorNext(&it);
        ASSERT(inf.synth_attribute_type == ATTR_CONSTANT_VALUE);
        ConstantPoolEntry entry = GetConstantPoolEntry(&cf, inf.data.constant_value.constantvalue_index);
        ASSERT(entry.tag == CONSTANT_String);
        ConstantPoolEntry tmp = GetConstantPoolEntry(&cf, entry.info.string_info.string_index);
        ASSERT(StringEquals(
            &tmp.info.utf8_info, "constant"));
        i++;
    }
    ASSERT(i == f.attributes_count);
    PASS();
}

SUITE(classfile_suite)
{
    SET_SETUP(Setup, NULL);
    SET_TEARDOWN(TearDown, NULL);
    RUN_TEST(TestValidateClassFileRead);
    RUN_TEST(TestGetClassFileBasicInformation);
    RUN_TEST(TestClassFileConstantPoolIterator);
    RUN_TEST(TestMethodInformation);
    RUN_TEST(TestMethodAttributeInformation);
    RUN_TEST(TestMethodQuickCodeLookup);
    RUN_TEST(TestFieldInformation);
    RUN_TEST(TestFieldAttributeInformation);
}
