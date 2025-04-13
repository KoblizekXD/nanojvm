#include <classparse.h>
#include <nanojvm.h>
#include <stdint.h>

/**
 * Internal bytecode executor. Will process instructions and
 * invoke actions for them accordingly.
 */
Item *execute_internal(VirtualMachine *vm, ThreadFrame *frame, ExStack *opstack, ExStack *lvars)
{
    CodeAttribute *code = frame->method->code;

    while (frame->pc < (code->code + code->code_length)) {
    
    }

    return NULL;
}
