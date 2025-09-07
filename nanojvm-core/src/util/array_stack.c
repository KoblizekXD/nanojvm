#include <util/array_stack.h>
#include <commons.h>

void ArrayStackPush(ArrayStack *stack, const void *data, const size_t data_size)
{
    if (stack->size + data_size > stack->capacity) {
        error("ArrayStack overflow: cannot push %zu bytes, only %zu bytes available",
              data_size, stack->capacity - stack->size);
        return;
    }
    MemoryCopy(stack->data + stack->size, data, data_size);
    stack->size += data_size;
}

void ArrayStackPop(ArrayStack *stack, void *out_data, const size_t data_size)
{
    if (data_size > stack->size) {
        error("ArrayStack underflow: cannot pop %zu bytes, only %zu bytes available",
              data_size, stack->size);
        return;
    }
    stack->size -= data_size;
    if (out_data != NULL) {
        MemoryCopy(out_data, stack->data + stack->size, data_size);
        MemorySet(stack->data + stack->size, 0, data_size);
    }
}

int8_t PopByte(ArrayStack *stack)
{
    int8_t data;
    ArrayStackPop(stack, &data, 4);
    return data;
}

int16_t PopShort(ArrayStack *stack)
{
    int16_t data;
    ArrayStackPop(stack, &data, 4);
    return data;
}

int32_t PopInt(ArrayStack *stack)
{
    int32_t data;
    ArrayStackPop(stack, &data, 4);
    return data;
}

int64_t PopLong(ArrayStack *stack)
{
    int64_t data;
    ArrayStackPop(stack, &data, 8);
    return data;
}

float PopFloat(ArrayStack *stack)
{
    float data;
    ArrayStackPop(stack, &data, sizeof(float));
    return data;
}

double PopDouble(ArrayStack *stack)
{
    double data;
    ArrayStackPop(stack, &data, sizeof(double));
    return data;
}

VirtualAddress PopAddress(ArrayStack *stack)
{
    VirtualAddress data;
    ArrayStackPop(stack, &data, sizeof(VirtualAddress));
    return data;
}

void PushInt(ArrayStack *stack, const int32_t data)
{
    ArrayStackPush(stack, &data, 4);
}

void PushLong(ArrayStack *stack, const int64_t data)
{
    ArrayStackPush(stack, &data, 8);
}

void PushFloat(ArrayStack *stack, const float data)
{
    ArrayStackPush(stack, &data, sizeof(float));
}

void PushDouble(ArrayStack *stack, const double data)
{
    ArrayStackPush(stack, &data, sizeof(double));
}

void PushAddress(ArrayStack *stack, const VirtualAddress data)
{
    ArrayStackPush(stack, &data, sizeof(VirtualAddress));
}