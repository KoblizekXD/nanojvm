#ifndef NANOJVM_MEMORY_OPSTACK_H
#define NANOJVM_MEMORY_OPSTACK_H

// Utilities for manipulating operand stacks of frames.

#include <stdint.h>
#include <nanojvm.h>

void PushInt(ThreadFrame *frame, int32_t value);
void PushShort(ThreadFrame *frame, int16_t value);
void PushByteCharBoolean(ThreadFrame *frame, int8_t value);
void PushLong(ThreadFrame *frame, int64_t value);
void PushDouble(ThreadFrame *frame, double value);
void PushFloat(ThreadFrame *frame, float value);
void PushReference(ThreadFrame *frame, void *value);

int32_t PopInt(ThreadFrame *frame);
int16_t PopShort(ThreadFrame *frame);
int8_t PopByteCharBoolean(ThreadFrame *frame);
int64_t PopLong(ThreadFrame *frame);
double PopDouble(ThreadFrame *frame);
float PopFloat(ThreadFrame *frame);
void *PopReference(ThreadFrame *frame);

#endif // NANOJVM_MEMORY_OPSTACK_H
