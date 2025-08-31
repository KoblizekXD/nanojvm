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

uint8_t PopByte(ArrayStack *stack)
{
    uint8_t data;
    ArrayStackPop(stack, &data, 4);
    return data;
}

uint16_t PopShort(ArrayStack *stack)
{
    uint16_t data;
    ArrayStackPop(stack, &data, 4);
    return data;
}

uint32_t PopInt(ArrayStack *stack)
{
    uint32_t data;
    ArrayStackPop(stack, &data, 4);
    return data;
}

uint64_t PopLong(ArrayStack *stack)
{
    uint64_t data;
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

void PushInt(ArrayStack *stack, const uint32_t data)
{
    ArrayStackPush(stack, &data, 4);
}

void PushLong(ArrayStack *stack, const uint64_t data)
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