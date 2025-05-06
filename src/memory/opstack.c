#include <memory/opstack.h>
#include <memory/utils.h>

#define STACK_PTR(frame) ((uint8_t *)((frame)->data + 4 * (frame)->local_size))

// Push* function assume that there is space for another element(s) to be pushed. Pushing long/double insert 2 elements instead(same applies for pop-like).

void PushInt(ThreadFrame *frame, int32_t value)
{
    uint8_t *stack = STACK_PTR(frame);
    memcpy(stack + 4 * frame->opstack_top, &value, 4);
    frame->opstack_top += 1;
}

void PushShort(ThreadFrame *frame, int16_t value)
{
    int32_t extended = (int32_t)value;
    PushInt(frame, extended);
}

void PushByteCharBoolean(ThreadFrame *frame, int8_t value)
{
    int32_t extended = (int32_t)value;
    PushInt(frame, extended);
}

void PushFloat(ThreadFrame *frame, float value)
{
    _Static_assert(sizeof(float) == 4, "Float must be 4 bytes");
    uint8_t *stack = STACK_PTR(frame);
    memcpy(stack + 4 * frame->opstack_top, &value, 4);
    frame->opstack_top += 1;
}

void PushLong(ThreadFrame *frame, int64_t value)
{
    uint8_t *stack = STACK_PTR(frame);
    memcpy(stack + 4 * frame->opstack_top, &value, 8);
    frame->opstack_top += 2;
}

void PushDouble(ThreadFrame *frame, double value)
{
    _Static_assert(sizeof(double) == 8, "Double must be 8 bytes");
    uint8_t *stack = STACK_PTR(frame);
    memcpy(stack + 4 * frame->opstack_top, &value, 8);
    frame->opstack_top += 2;
}

void PushReference(ThreadFrame *frame, void *value)
{
    uint8_t *stack = STACK_PTR(frame);
    memcpy(stack + 4 * frame->opstack_top, &value, sizeof(uintptr_t));
    frame->opstack_top += sizeof(uintptr_t);
}

// Pop* functions do a simple assumption too: There is always required amount of elements which is required to be popped

int32_t PopInt(ThreadFrame *frame)
{
    frame->opstack_top -= 1;
    int32_t value;
    memcpy(&value, STACK_PTR(frame) + 4 * frame->opstack_top, 4);
    return value;
}

int16_t PopShort(ThreadFrame *frame)
{
    return (int16_t) PopInt(frame);
}

int8_t PopByteCharBoolean(ThreadFrame *frame) {
    return (int8_t)PopInt(frame);
}

float PopFloat(ThreadFrame *frame)
{
    frame->opstack_top -= 1;
    float value;
    memcpy(&value, STACK_PTR(frame) + 4 * frame->opstack_top, 4);
    return value;
}

int64_t PopLong(ThreadFrame *frame)
{
    frame->opstack_top -= 2;
    int64_t value;
    memcpy(&value, STACK_PTR(frame) + 4 * frame->opstack_top, 8);
    return value;
}

double PopDouble(ThreadFrame *frame)
{
    frame->opstack_top -= 2;
    double value;
    memcpy(&value, STACK_PTR(frame) + 4 * frame->opstack_top, 8);
    return value;
}

void *PopReference(ThreadFrame *frame)
{
    frame->opstack_top -= sizeof(uintptr_t);
    void *value = NULL;
    memcpy(&value, STACK_PTR(frame) + 4 * frame->opstack_top, sizeof(uintptr_t));
    return value;
}
