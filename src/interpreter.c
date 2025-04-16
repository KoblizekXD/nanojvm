#include <mem/exstack.h>
#include <stdlib.h>
#include <util/strings.h>
#include <vmopts.h>
#include <string.h>
#include <classparse.h>
#include <nanojvm.h>
#include <objects.h>
#include <stdint.h>
#include <util/logging.h>

#define Read16() (*frame->pc << 8) | *(frame->pc + 1); frame->pc += 2
#define Read8() *frame->pc++

#define INSTRUCTION_PARAMS VirtualMachine *vm, ThreadFrame *frame, ExStack *opstack, ExStack *lvars
#define PASS_PARAMS vm, frame, opstack, lvars
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

static inline void ldc(INSTRUCTION_PARAMS, uint16_t index)
{
    ConstantPoolEntry entry = frame->method->cf->constant_pool[index - 1];
    switch (entry.tag) {
        case CONSTANT_Integer:
            PushInt(opstack, (int32_t) entry.info.int_float);
            break;
        case CONSTANT_Float:
            PushInt(opstack, (float) entry.info.int_float);
            break;
        case CONSTANT_Class:
            error("Support for java/lang/Class initialization has not yet been implemented");
            break;
        case CONSTANT_String:
            PushReference(opstack, InstantiateString(vm, *entry.info.string.string));
            break;
        default:
            error("Unresolved LDC constant: %d", entry.tag);
            break;
    }
}

INSTRUCTION(ldc)
{
    uint8_t index = Read8();
    ldc(PASS_PARAMS, index);
}

INSTRUCTION(ldc_w)
{
    uint16_t index = Read16();
    ldc(PASS_PARAMS, index);
}

INSTRUCTION(ldc2_w)
{
    uint16_t index = Read16();
    // TODO
}

static inline void load(INSTRUCTION_PARAMS, uint16_t index, uint8_t met)
{
    PushStack(opstack, Copy(lvars->data[index]));
}

INSTRUCTION(iload)
{
    load(PASS_PARAMS, Read8(), STACK_ELEMENT_INT);
}

INSTRUCTION(lload)
{
    load(PASS_PARAMS, Read8(), STACK_ELEMENT_LONG);
}

INSTRUCTION(fload)
{
    load(PASS_PARAMS, Read8(), STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(dload)
{
    load(PASS_PARAMS, Read8(), STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(aload)
{
    load(PASS_PARAMS, Read8(), STACK_ELEMENT_LONG | STACK_ELEMENT_IS_ADDRESS);
}

INSTRUCTION(iload_0)
{
    load(PASS_PARAMS, 0, STACK_ELEMENT_INT);
}

INSTRUCTION(iload_1)
{
    load(PASS_PARAMS, 1, STACK_ELEMENT_INT);
}

INSTRUCTION(iload_2)
{
    load(PASS_PARAMS, 2, STACK_ELEMENT_INT);
}

INSTRUCTION(iload_3)
{
    load(PASS_PARAMS, 3, STACK_ELEMENT_INT);
}

INSTRUCTION(lload_0)
{
    load(PASS_PARAMS, 0, STACK_ELEMENT_LONG);
}

INSTRUCTION(lload_1)
{
    load(PASS_PARAMS, 1, STACK_ELEMENT_LONG);
}

INSTRUCTION(lload_2)
{
    load(PASS_PARAMS, 2, STACK_ELEMENT_LONG);
}

INSTRUCTION(lload_3)
{
    load(PASS_PARAMS, 3, STACK_ELEMENT_LONG);
}

INSTRUCTION(fload_0)
{
    load(PASS_PARAMS, 0, STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(fload_1)
{
    load(PASS_PARAMS, 1, STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(fload_2)
{
    load(PASS_PARAMS, 2, STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(fload_3)
{
    load(PASS_PARAMS, 3, STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(dload_0)
{
    load(PASS_PARAMS, 0, STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(dload_1)
{
    load(PASS_PARAMS, 1, STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(dload_2)
{
    load(PASS_PARAMS, 2, STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(dload_3)
{
    load(PASS_PARAMS, 3, STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING);
}

INSTRUCTION(aload_0)
{
    load(PASS_PARAMS, 0, STACK_ELEMENT_LONG | STACK_ELEMENT_IS_ADDRESS);
}

INSTRUCTION(aload_1)
{
    load(PASS_PARAMS, 1, STACK_ELEMENT_LONG | STACK_ELEMENT_IS_ADDRESS);
}

INSTRUCTION(aload_2)
{
    load(PASS_PARAMS, 2, STACK_ELEMENT_LONG | STACK_ELEMENT_IS_ADDRESS);
}

INSTRUCTION(aload_3)
{
    load(PASS_PARAMS, 3, STACK_ELEMENT_LONG | STACK_ELEMENT_IS_ADDRESS);
}

void arrayload(INSTRUCTION_PARAMS)
{
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    PushStack(opstack, GetArrayValue(arrayref, index));
}

INSTRUCTION(iaload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(laload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(faload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(daload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(aaload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(baload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(caload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(saload)
{
    arrayload(PASS_PARAMS);
}

INSTRUCTION(istore)
{
    uint8_t index = Read8();
    int32_t i = PopInt(opstack);
    free(lvars->data[index]);
    lvars->data[index] = CreateItem(STACK_ELEMENT_INT, &i);
}

INSTRUCTION(fstore)
{
    uint8_t index = Read8();
    float f = PopFloat(opstack);
    free(lvars->data[index]);
    lvars->data[index] = CreateItem(STACK_ELEMENT_FLOATING | STACK_ELEMENT_INT, &f);
}

INSTRUCTION(astore)
{
    uint8_t index = Read8();
    void* ref = PopReference(opstack);
    free(lvars->data[index]);
    lvars->data[index] = CreateItem(STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG, ref);
}

INSTRUCTION(lstore)
{
    uint8_t index = Read8();
    int64_t l = PopLong(opstack);
    free(lvars->data[index]);
    lvars->data[index] = CreateItem(STACK_ELEMENT_LONG, &l);
}

INSTRUCTION(dstore)
{
    uint8_t index = Read8();
    double d = PopDouble(opstack);
    free(lvars->data[index]);
    lvars->data[index] = CreateItem(STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING, &d);
}

/**
 * Internal bytecode executor. Will process instructions and
 * invoke actions for them accordingly.
 */
Item *execute_internal(VirtualMachine *vm, ThreadFrame *frame, ExStack *opstack, ExStack *lvars)
{
    CodeAttribute *code = frame->method->code;

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
            HANDLER_FOR(ILOAD, iload);
            HANDLER_FOR(LLOAD, lload);
            HANDLER_FOR(FLOAD, fload);
            HANDLER_FOR(DLOAD, dload);
            HANDLER_FOR(ALOAD, aload);
            HANDLER_FOR(ILOAD_0, iload_0);
            HANDLER_FOR(ILOAD_1, iload_1);
            HANDLER_FOR(ILOAD_2, iload_2);
            HANDLER_FOR(ILOAD_3, iload_3);
            HANDLER_FOR(LLOAD_0, lload_0);
            HANDLER_FOR(LLOAD_1, lload_1);
            HANDLER_FOR(LLOAD_2, lload_2);
            HANDLER_FOR(LLOAD_3, lload_3);
            HANDLER_FOR(FLOAD_0, fload_0);
            HANDLER_FOR(FLOAD_1, fload_1);
            HANDLER_FOR(FLOAD_2, fload_2);
            HANDLER_FOR(FLOAD_3, fload_3);
            HANDLER_FOR(DLOAD_0, dload_0);
            HANDLER_FOR(DLOAD_1, dload_1);
            HANDLER_FOR(DLOAD_2, dload_2);
            HANDLER_FOR(DLOAD_3, dload_3);
            HANDLER_FOR(ALOAD_0, aload_0);
            HANDLER_FOR(ALOAD_1, aload_1);
            HANDLER_FOR(ALOAD_2, aload_2);
            HANDLER_FOR(ALOAD_3, aload_3);
            HANDLER_FOR(IALOAD, iaload);
            HANDLER_FOR(LALOAD, laload);
            HANDLER_FOR(FALOAD, faload);
            HANDLER_FOR(DALOAD, daload);
            HANDLER_FOR(AALOAD, aaload);
            HANDLER_FOR(BALOAD, baload);
            HANDLER_FOR(CALOAD, caload);
            HANDLER_FOR(SALOAD, saload);
            HANDLER_FOR(ISTORE, istore);
            HANDLER_FOR(LSTORE, lstore);
            HANDLER_FOR(FSTORE, fstore);
            HANDLER_FOR(DSTORE, dstore);
            HANDLER_FOR(ASTORE, astore);
            default:
                ThrowException(vm, "java/lang/InternalError", "Unresolved instruction: %s - 0x%X", GetInstructionName(opcode), opcode);
                break;
        }
    }

    return NULL;
}
