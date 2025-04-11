#include "util/exstack.h"
#include "util/logging.h"
#include <stdlib.h>
#include <util/assertions.h>

static inline void AssertVariableMetadata(Item *var, uint8_t metadata)
{
    if ((var->metadata & metadata) != metadata) {
        error("Internal assertion failed");
        exit(1);
    }
}

void AssertByte(Item *var)
{
    AssertVariableMetadata(var, STACK_ELEMENT_BYTE);
}

void AssertShort(Item *var)
{
    AssertVariableMetadata(var, STACK_ELEMENT_SHORT);
}

void AssertInt(Item *var)
{
    AssertVariableMetadata(var, STACK_ELEMENT_INT);
}

void AssertLong(Item *var)
{
    AssertVariableMetadata(var, STACK_ELEMENT_LONG);
}

void AssertFloating(Item *var)
{
    AssertVariableMetadata(var, STACK_ELEMENT_FLOATING);
}

void AssertReference(Item *var)
{
    AssertVariableMetadata(var, STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG);
}

void AssertCanPush(ExStack *stack)
{
    if (stack->top >= stack->capacity) {
        error("Internal assertion failed");
        exit(1);
    }
}
