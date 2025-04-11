#ifndef NANOJVM_UTIL_ASSERTIONS_H
#define NANOJVM_UTIL_ASSERTIONS_H

#include <util/exstack.h>

static inline void AssertVariableMetadata(Item *var, uint8_t metadata);
void AssertByte(Item *var);
void AssertShort(Item *var);
void AssertInt(Item *var);
void AssertLong(Item *var);
void AssertFloating(Item *var);
void AssertReference(Item *var);
void AssertCanPush(ExStack *stack);

#endif
