#ifndef NANOJVM_MEM_EXSTACK_H
#define NANOJVM_MEM_EXSTACK_H

#include <stddef.h>
#include <stdint.h>

// Item metadata
#define STACK_ELEMENT_BYTE   0x0  // 0b00 - byte, char, boolean
#define STACK_ELEMENT_SHORT  0x1  // 0b01 - 16bit integer - short
#define STACK_ELEMENT_INT    0x2  // 0b10 - 32bit integer - int
#define STACK_ELEMENT_LONG   0x3  // 0b11 - 64bit integer - long

#define STACK_ELEMENT_IS_ADDRESS (1 << 2) // Must also have STACK_ELEMENT_LONG set
#define STACK_ELEMENT_FLOATING (1 << 3)   // Optional for either STACK_ELEMENT_LONG or STACK_ELEMENT_INT. Will mark as double or float consecutively.

// Wrapper for local variables, stack and more...
typedef struct exstack_item {
    uint8_t metadata; // Described in above present macros 
    uint8_t data[];
} Item;

typedef struct exstack {
    size_t capacity;
    size_t top; // Last free element
    Item **data;
} ExStack;

/**
 * Returns the size of the data in bytes, will be one of: 1, 2, 4, 8 bytes.
 */
size_t   GetItemValueSize(Item *item);
ExStack *CreateStack(size_t capacity);
void     DestroyStack(ExStack *stack);

/**
 * Cleans the stack structure and resets the top cursor.
 */
void     CleanStack(ExStack *stack);

/**
 * Pushes item to the stack. If the stack is full, error is logged.
 */
void     PushStack(ExStack *stack, Item *item);

/**
 * Pops item from the stack, if no items are present, error is logged.
 */
Item    *PopStack(ExStack *stack);

/**
 * Create item inline, so you don't have to do it manually :)
 */
Item *CreateItem(uint8_t metadata, void *source);

/**
 * Duplicates given item.
 */
Item *Copy(Item *src);

// Utilities to manage the stack structure

int8_t  _pop_byte(ExStack *stack);
int16_t _pop_short(ExStack *stack);
int32_t _pop_int(ExStack *stack);
int64_t _pop_long(ExStack *stack);
float   _pop_float(ExStack *stack);
double  _pop_double(ExStack *stack);
void   *_pop_reference(ExStack *stack);

void _push_byte(ExStack *stack, int8_t value);
void _push_short(ExStack *stack, int16_t value);
void _push_int(ExStack *stack, int32_t value);
void _push_long(ExStack *stack, int64_t value);
void _push_float(ExStack *stack, float value);
void _push_double(ExStack *stack, double value);
void _push_reference(ExStack *stack, void *value);

#define PopByte(stack) _pop_byte(stack)
#define PopShort(stack) _pop_short(stack)
#define PopInt(stack) _pop_int(stack)
#define PopLong(stack) _pop_long(stack)
#define PopFloat(stack) _pop_float(stack)
#define PopDouble(stack) _pop_double(stack)
#define PopReference(stack) _pop_reference(stack)

#define PushByte(stack, value) _push_byte(stack, value)
#define PushShort(stack, value) _push_short(stack, value)
#define PushInt(stack, value) _push_int(stack, value)
#define PushLong(stack, value) _push_long(stack, value)
#define PushFloat(stack, value) _push_float(stack, value)
#define PushDouble(stack, value) _push_double(stack, value)
#define PushReference(stack, value) _push_reference(stack, value)

#endif
