#include <assert.h>
#include <stdlib.h>
#include <mem/exstack.h>
#include <util/logging.h>
#include <util/assertions.h>

ExStack *CreateStack(size_t capacity)
{
    ExStack *stack = malloc(sizeof(ExStack));
    stack->capacity = capacity;
    stack->top = 0;
    stack->data = calloc(capacity, sizeof(Item*));

    return stack;
}

void DestroyStack(ExStack *stack)
{
    if (stack == NULL) return;

    for (size_t i = 0; i < stack->capacity; i++) {
        free(stack->data[i]);
    }
    free(stack->data);
    stack->data = NULL;
    stack->top = 0;
    stack->capacity = 0;
    free(stack);
}

void CleanStack(ExStack *stack)
{
    if (stack == NULL) return;
    for (size_t i = 0; i < stack->top; i++) {
        free(stack->data[i]);
        stack->data[i] = NULL;
    }
}

void PushStack(ExStack *stack, Item *item)
{
    if (stack->top == stack->capacity) {
        error("Cannot push to stack: Insufficent capacity");
        return;
    }

    stack->data[stack->top] = item;
    stack->top++;
}

Item *PopStack(ExStack *stack)
{
    if (stack == NULL || stack->top == 0) {
        error("Cannot pop from stack: Stack is empty or uninitialized");
        return NULL;
    }

    Item *i = stack->data[--stack->top];
    stack->data[stack->top] = NULL;
    return i;
}

size_t GetItemValueSize(Item *item)
{
    switch (item->metadata & 0x3) {
        case STACK_ELEMENT_LONG:  return 8;
        case STACK_ELEMENT_INT:   return 4;
        case STACK_ELEMENT_SHORT: return 2;
        case STACK_ELEMENT_BYTE:  return 1;
        default: return 0; 
    }
}

int8_t _pop_byte(ExStack *stack)
{
    Item *v = PopStack(stack);
    AssertByte(v);
    int8_t value;
    memcpy(&value, v->data, 1);
    free(v);
    return value;
}

int16_t _pop_short(ExStack *stack)
{
    Item *v = PopStack(stack);
    AssertShort(v);
    int16_t value;
    memcpy(&value, v->data, 2);
    free(v);
    return value;
}

int32_t _pop_int(ExStack *stack)
{
    Item *v = PopStack(stack);
    AssertInt(v);
    int32_t value;
    memcpy(&value, v->data, 4);
    free(v);
    return value;
}

int64_t _pop_long(ExStack *stack)
{
    Item *v = PopStack(stack);
    AssertLong(v);
    int64_t value;
    memcpy(&value, v->data, 8);
    free(v);
    return value;
}

float _pop_float(ExStack *stack)
{
    Item *v = PopStack(stack);
    AssertFloating(v);
    float value;
    memcpy(&value, v->data, 4);
    free(v);
    return value;
}

double _pop_double(ExStack *stack)
{
    Item *v = PopStack(stack);
    AssertFloating(v);
    double value;
    memcpy(&value, v->data, 8);
    free(v);
    return value;
}

void *_pop_reference(ExStack *stack)
{
    Item *v = PopStack(stack);
    AssertReference(v);
    void *value;
    memcpy(&value, v->data, 8);
    free(v);
    return value;
}

void _push_byte(ExStack *stack, int8_t value)
{
    AssertCanPush(stack);
    Item *v = malloc(sizeof(Item) + 1);
    v->metadata = STACK_ELEMENT_BYTE;
    memcpy(v->data, &value, 1);
    stack->data[stack->top++] = v;
}

void _push_short(ExStack *stack, int16_t value)
{
    AssertCanPush(stack);
    Item *v = malloc(sizeof(Item) + 2);
    v->metadata = STACK_ELEMENT_SHORT;
    memcpy(v->data, &value, 2);
    stack->data[stack->top++] = v;

}

void _push_int(ExStack *stack, int32_t value)
{
    AssertCanPush(stack);
    Item *v = malloc(sizeof(Item) + 4);
    v->metadata = STACK_ELEMENT_INT;
    memcpy(v->data, &value, 4);
    stack->data[stack->top++] = v;
}

void _push_long(ExStack *stack, int64_t value)
{
    AssertCanPush(stack);
    Item *v = malloc(sizeof(Item) + 8);
    v->metadata = STACK_ELEMENT_LONG;
    memcpy(v->data, &value, 8);
    stack->data[stack->top++] = v;
}

void _push_float(ExStack *stack, float value)
{
    AssertCanPush(stack);
    Item *v = malloc(sizeof(Item) + 4);
    v->metadata = STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING;
    memcpy(v->data, &value, 4);
    stack->data[stack->top++] = v;
}

void _push_double(ExStack *stack, double value)
{
    AssertCanPush(stack);
    Item *v = malloc(sizeof(Item) + 8);
    v->metadata = STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING;
    memcpy(v->data, &value, 8);
    stack->data[stack->top++] = v;
}

void _push_reference(ExStack *stack, void *value)
{
    AssertCanPush(stack);
    Item *v = malloc(sizeof(Item) + 8);
    v->metadata = STACK_ELEMENT_LONG | STACK_ELEMENT_IS_ADDRESS;
    memcpy(v->data, &value, 8);
    stack->data[stack->top++] = v;
}
