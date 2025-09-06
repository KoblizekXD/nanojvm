#ifndef NANOJVM_CORE_UTIL_ARRAY_STACK_H
#define NANOJVM_CORE_UTIL_ARRAY_STACK_H

#include <stdint.h>
#include <stddef.h>

/**
 * A general purpose stack-like structure backed by a fixed-size array.
 * It supports pushing and popping of arbitrary data types by specifying their size.
 */
typedef struct njvmArrayStack {
    size_t size;
    size_t capacity;
    uint8_t *data;
} ArrayStack;

/**
 * Create an ArrayStack with a fixed capacity.
 * @param name the name of variable
 * @param capacity the capacity of the stack in bytes(multiply by 4 for number of normal operand stack entries)
 */
#define CreateArrayStack(name, capacity) \
    uint8_t name##_data[capacity]; \
    ArrayStack name = {0, capacity, name##_data}

void ArrayStackPush(ArrayStack *stack, const void *data, size_t data_size);
void ArrayStackPop(ArrayStack *stack, void *out_data, size_t data_size);

void ArrayStackClear(ArrayStack *stack);

uint8_t PopByte(ArrayStack *stack);
uint16_t PopShort(ArrayStack *stack);
uint32_t PopInt(ArrayStack *stack);
uint64_t PopLong(ArrayStack *stack);
float PopFloat(ArrayStack *stack);
double PopDouble(ArrayStack *stack);

// Use PushInt also for char, boolean, byte and short, as they are all 4 bytes in the stack
void PushInt(ArrayStack *stack, uint32_t data);
void PushLong(ArrayStack *stack, uint64_t data);
void PushFloat(ArrayStack *stack, float data);
void PushDouble(ArrayStack *stack, double data);

#endif // NANOJVM_CORE_UTIL_ARRAY_STACK_H