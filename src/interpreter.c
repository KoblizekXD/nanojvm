#include <mem/exstack.h>
#include <util/strings.h>
#include <vmopts.h>
#include <string.h>
#include <classparse.h>
#include <nanojvm.h>
#include <objects.h>
#include <stdint.h>

#define Read16() (*frame->pc << 8) | *(frame->pc + 1); frame->pc += 2
#define Read8() *frame->pc++

#define INSTRUCTION(NAME) static inline void handler_##NAME(VirtualMachine *vm, ThreadFrame *frame, ExStack *opstack, ExStack *lvars)
#define HANDLER_FOR(INSN, HANDLER) case INSN: handler_##HANDLER(vm, frame, opstack, lvars); break;

INSTRUCTION(aconst_null)
{
    PushReference(opstack, NULL);
}

INSTRUCTION(iconst_m1)
{
    PushInt(opstack, -1);
}

INSTRUCTION(iconst_0)
{
    PushInt(opstack, 0);
}

INSTRUCTION(iconst_1)
{
    PushInt(opstack, 1);
}

INSTRUCTION(iconst_2)
{
    PushInt(opstack, 2);
}

INSTRUCTION(iconst_3)
{
    PushInt(opstack, 3);
}

INSTRUCTION(iconst_4)
{
    PushInt(opstack, 4);
}

INSTRUCTION(iconst_5)
{
    PushInt(opstack, 5);
}

INSTRUCTION(lconst_0)
{
    PushLong(opstack, 0);
}

INSTRUCTION(lconst_1)
{
    PushLong(opstack, 1);
}

INSTRUCTION(fconst_0)
{
    PushFloat(opstack, 0.0f);
}

INSTRUCTION(fconst_1)
{
    PushFloat(opstack, 1.0f);
}

INSTRUCTION(fconst_2)
{
    PushFloat(opstack, 2.0f);
}

INSTRUCTION(dconst_0)
{
    PushDouble(opstack, 0.0);
}

INSTRUCTION(dconst_1)
{
    PushDouble(opstack, 1.0);
}

INSTRUCTION(bipush)
{
    PushByte(opstack, (int8_t) Read8());
}

INSTRUCTION(sipush)
{
    uint16_t value = Read16();
    PushShort(opstack, (int16_t) value);
}

INSTRUCTION(ldc)
{
    uint8_t index = Read8();

}

INSTRUCTION(ldc_w)
{
    uint16_t index = Read16();
}

INSTRUCTION(ldc2_w)
{
    uint16_t index = Read16();
}

/**
 * Internal bytecode executor. Will process instructions and
 * invoke actions for them accordingly.
 */
Item *execute_internal(VirtualMachine *vm, ThreadFrame *frame, ExStack *opstack, ExStack *lvars)
{
    CodeAttribute *code = frame->method->code;
    ClassFile *cf = frame->method->cf;

    if (setjmp(frame->ret_buf) == 0)
        frame->pc = code->code;
    else {
        frame = &FrameCeiling(GetCurrent(vm));
    }

    while (frame->pc < (code->code + code->code_length)) {
        uint8_t opcode = Read8();
        switch (opcode) {
            case NOP: break;
            HANDLER_FOR(ACONST_NULL, aconst_null);
            HANDLER_FOR(ICONST_M1, iconst_m1);
            HANDLER_FOR(ICONST_0, iconst_0);
            HANDLER_FOR(ICONST_1, iconst_1);
            HANDLER_FOR(ICONST_2, iconst_2);
            HANDLER_FOR(ICONST_3, iconst_3);
            HANDLER_FOR(ICONST_4, iconst_4);
            HANDLER_FOR(ICONST_5, iconst_5);
            HANDLER_FOR(LCONST_0, lconst_0);
            HANDLER_FOR(LCONST_1, lconst_1);
            HANDLER_FOR(FCONST_0, fconst_0);
            HANDLER_FOR(FCONST_1, fconst_1);
            HANDLER_FOR(FCONST_2, fconst_2);
            HANDLER_FOR(DCONST_0, dconst_0);
            HANDLER_FOR(DCONST_1, dconst_1);
            HANDLER_FOR(BIPUSH, bipush);
            HANDLER_FOR(SIPUSH, sipush);
            HANDLER_FOR(LDC, ldc);
            HANDLER_FOR(LDC_W, ldc_w);
            HANDLER_FOR(LDC2_W, ldc2_w);
            default:
                ThrowException(vm, "java/lang/InternalError", "Unresolved instruction: %s - 0x%X", GetInstructionName(opcode), opcode);
                break;
        }
    }

    return NULL;
}
