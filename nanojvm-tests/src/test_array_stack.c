#include <greatest.h>
#include <util/array_stack.h>

TEST TestBasicStackOperations(void)
{
    CreateArrayStack(testing, 8);
    ASSERT(testing.size == 0 && testing.capacity == 8);
    ArrayStackPush(&testing, &(uint32_t){0xDEADBEEF}, 4);
    ASSERT(testing.size == 4);
    ArrayStackPush(&testing, &(uint32_t){0xCAFEBABE}, 4);
    ASSERT(testing.size == testing.capacity);
    uint32_t data;
    ArrayStackPop(&testing, &data, 4);
    ASSERT(data == 0xCAFEBABE);
    ASSERT(testing.size == 4);
    ArrayStackPop(&testing, &data, 4);
    ASSERT(data == 0xDEADBEEF);
    ASSERT(testing.size == 0);
    PASS();
}

TEST TestIntegralUtilityFunctions(void)
{
    CreateArrayStack(testing, 12);
    PushInt(&testing, 42);
    PushLong(&testing, 0x1122334455667788);
    ASSERT(testing.size == 12);
    ASSERT(PopLong(&testing) == 0x1122334455667788);
    ASSERT(testing.size == 4);
    ASSERT(PopInt(&testing) == 42);
    ASSERT(testing.size == 0);
    PASS();
}

TEST TestFloatingPointUtilityFunctions(void)
{
    CreateArrayStack(testing, 12);
    PushFloat(&testing, 3.14f);
    PushDouble(&testing, 2.718281828459045);
    ASSERT(testing.size == 12);
    ASSERT(PopDouble(&testing) == 2.718281828459045);
    ASSERT(testing.size == 4);
    ASSERT(PopFloat(&testing) == 3.14f);
    ASSERT(testing.size == 0);
    PASS();
}

TEST TestIntLongManipulation(void)
{
    CreateArrayStack(testing, 8);
    PushInt(&testing, 0xAA55);
    PushInt(&testing, 0x55AA);
    const int64_t l = PopLong(&testing);
    PushLong(&testing, l);
    ASSERT(PopInt(&testing) == 0x55AA);
    ASSERT(PopInt(&testing) == 0xAA55);
    PASS();
}

SUITE(array_stack_suite)
{
    RUN_TEST(TestBasicStackOperations);
    RUN_TEST(TestIntegralUtilityFunctions);
    RUN_TEST(TestFloatingPointUtilityFunctions);
    RUN_TEST(TestIntLongManipulation);
}