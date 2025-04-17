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

INSTRUCTION(istore_0)
{
    int32_t i = PopInt(opstack);
    free(lvars->data[0]);
    lvars->data[0] = CreateItem(STACK_ELEMENT_INT, &i);
}

INSTRUCTION(istore_1)
{
    int32_t i = PopInt(opstack);
    free(lvars->data[1]);
    lvars->data[1] = CreateItem(STACK_ELEMENT_INT, &i);
}

INSTRUCTION(istore_2)
{
    int32_t i = PopInt(opstack);
    free(lvars->data[2]);
    lvars->data[2] = CreateItem(STACK_ELEMENT_INT, &i);
}

INSTRUCTION(istore_3)
{
    int32_t i = PopInt(opstack);
    free(lvars->data[3]);
    lvars->data[3] = CreateItem(STACK_ELEMENT_INT, &i);
}

INSTRUCTION(fstore_0)
{
    float f = PopFloat(opstack);
    free(lvars->data[0]);
    lvars->data[0] = CreateItem(STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING, &f);
}

INSTRUCTION(fstore_1)
{
    float f = PopFloat(opstack);
    free(lvars->data[1]);
    lvars->data[1] = CreateItem(STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING, &f);
}

INSTRUCTION(fstore_2)
{
    float f = PopFloat(opstack);
    free(lvars->data[2]);
    lvars->data[2] = CreateItem(STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING, &f);
}

INSTRUCTION(fstore_3)
{
    float f = PopFloat(opstack);
    free(lvars->data[3]);
    lvars->data[3] = CreateItem(STACK_ELEMENT_INT | STACK_ELEMENT_FLOATING, &f);
}

INSTRUCTION(dstore_0)
{
    double d = PopDouble(opstack);
    free(lvars->data[0]);
    lvars->data[0] = CreateItem(STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING, &d);
}

INSTRUCTION(dstore_1)
{
    double d = PopDouble(opstack);
    free(lvars->data[1]);
    lvars->data[1] = CreateItem(STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING, &d);
}

INSTRUCTION(dstore_2)
{
    double d = PopDouble(opstack);
    free(lvars->data[2]);
    lvars->data[2] = CreateItem(STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING, &d);
}

INSTRUCTION(dstore_3)
{
    double d = PopDouble(opstack);
    free(lvars->data[3]);
    lvars->data[3] = CreateItem(STACK_ELEMENT_LONG | STACK_ELEMENT_FLOATING, &d);
}

INSTRUCTION(astore_0)
{
    void* ref = PopReference(opstack);
    free(lvars->data[0]);
    lvars->data[0] = CreateItem(STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG, ref);
}

INSTRUCTION(astore_1)
{
    void* ref = PopReference(opstack);
    free(lvars->data[1]);
    lvars->data[1] = CreateItem(STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG, ref);
}

INSTRUCTION(astore_2)
{
    void* ref = PopReference(opstack);
    free(lvars->data[2]);
    lvars->data[2] = CreateItem(STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG, ref);
}

INSTRUCTION(astore_3)
{
    void* ref = PopReference(opstack);
    free(lvars->data[3]);
    lvars->data[3] = CreateItem(STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG, ref);
}

INSTRUCTION(lstore_0)
{
    int64_t l = PopLong(opstack);
    free(lvars->data[0]);
    lvars->data[0] = CreateItem(STACK_ELEMENT_LONG, &l);
}

INSTRUCTION(lstore_1)
{
    int64_t l = PopLong(opstack);
    free(lvars->data[1]);
    lvars->data[1] = CreateItem(STACK_ELEMENT_LONG, &l);
}

INSTRUCTION(lstore_2)
{
    int64_t l = PopLong(opstack);
    free(lvars->data[2]);
    lvars->data[2] = CreateItem(STACK_ELEMENT_LONG, &l);
}

INSTRUCTION(lstore_3)
{
    int64_t l = PopLong(opstack);
    free(lvars->data[3]);
    lvars->data[3] = CreateItem(STACK_ELEMENT_LONG, &l);
}

INSTRUCTION(iastore)
{
    int32_t value = PopInt(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    SetArrayValue(arrayref, index, &value);
}

INSTRUCTION(fastore)
{
    float value = PopFloat(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    SetArrayValue(arrayref, index, &value);
}

INSTRUCTION(lastore)
{
    int64_t value = PopLong(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    SetArrayValue(arrayref, index, &value);
}

INSTRUCTION(dastore)
{
    double value = PopDouble(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    SetArrayValue(arrayref, index, &value);
}

INSTRUCTION(aastore)
{
    void *ref = PopReference(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    SetArrayValue(arrayref, index, ref);
}

INSTRUCTION(bastore)
{
    int32_t value = PopInt(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    int8_t byteValue = (int8_t)value;
    SetArrayValue(arrayref, index, &byteValue);
}

INSTRUCTION(castore)
{
    int32_t value = PopInt(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    uint16_t charValue = (uint16_t)value;
    SetArrayValue(arrayref, index, &charValue);
}

INSTRUCTION(sastore)
{
    int32_t value = PopInt(opstack);
    int32_t index = PopInt(opstack);
    HeapRegion *arrayref = PopReference(opstack);
    int16_t shortValue = (int16_t)value;  // truncate
    SetArrayValue(arrayref, index, &shortValue);
}

INSTRUCTION(pop)
{
    free(PopStack(opstack));
}

INSTRUCTION(pop2)
{
    Item *value1 = PopStack(opstack);
    if (value1->metadata & STACK_ELEMENT_LONG && !(value1->metadata & STACK_ELEMENT_IS_ADDRESS))
        free(value1);
    else {
        free(value1);
        free(PopStack(opstack));
    }
}

INSTRUCTION(dup)
{
    Item *i = opstack->data[opstack->top - 1];
    PushStack(opstack, Copy(i));
}

INSTRUCTION(dup_x1)
{
    Item *value1 = PopStack(opstack);
    Item *value2 = PopStack(opstack);
    PushStack(opstack, Copy(value1));
    PushStack(opstack, value2);
    PushStack(opstack, value1);
}

INSTRUCTION(dup_x2)
{
    Item *value1 = PopStack(opstack);

    Item *value2 = PopStack(opstack);
    if (value2->metadata & STACK_ELEMENT_LONG && !(value2->metadata & STACK_ELEMENT_IS_ADDRESS)) {
        PushStack(opstack, Copy(value1));
        PushStack(opstack, value2);
        PushStack(opstack, value1);
    } else {
        Item *value3 = PopStack(opstack);
        PushStack(opstack, Copy(value1));
        PushStack(opstack, value3);
        PushStack(opstack, value2);
        PushStack(opstack, value1);
    }
}

INSTRUCTION(dup2)
{
    Item *value1 = PopStack(opstack);
    
    if (value1->metadata & STACK_ELEMENT_LONG && !(value1->metadata & STACK_ELEMENT_IS_ADDRESS)) {
        PushStack(opstack, Copy(value1));
        PushStack(opstack, value1);
    } else {
        Item *value2 = PopStack(opstack);
        PushStack(opstack, Copy(value2));
        PushStack(opstack, Copy(value1));
        PushStack(opstack, value2);
        PushStack(opstack, value1);
    }
}

INSTRUCTION(dup2_x1)
{
    Item *value1 = PopStack(opstack);

    if (value1->metadata & STACK_ELEMENT_LONG && !(value1->metadata & STACK_ELEMENT_IS_ADDRESS)) {
        Item *value2 = PopStack(opstack);
        PushStack(opstack, Copy(value1));
        PushStack(opstack, value2);
        PushStack(opstack, value1);
    } else {
        Item *value2 = PopStack(opstack);
        Item *value3 = PopStack(opstack);
        PushStack(opstack, Copy(value2));
        PushStack(opstack, Copy(value1));
        PushStack(opstack, value3);
        PushStack(opstack, value2);
        PushStack(opstack, value1);
    }
}

INSTRUCTION(dup2_x2)
{
    error("I don't actually like dup2_x2 that much, if you read this, report it as a bug");
}

INSTRUCTION(swap)
{
    Item *value1 = PopStack(opstack);
    Item *value2 = PopStack(opstack);
    PushStack(opstack, value1);
    PushStack(opstack, value2);
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
            HANDLER_FOR(ISTORE_0, istore_0);
            HANDLER_FOR(ISTORE_1, istore_1);
            HANDLER_FOR(ISTORE_2, istore_2);
            HANDLER_FOR(ISTORE_3, istore_3);
            HANDLER_FOR(LSTORE_0, lstore_0);
            HANDLER_FOR(LSTORE_1, lstore_1);
            HANDLER_FOR(LSTORE_2, lstore_2);
            HANDLER_FOR(LSTORE_3, lstore_3);
            HANDLER_FOR(FSTORE_0, fstore_0);
            HANDLER_FOR(FSTORE_1, fstore_1);
            HANDLER_FOR(FSTORE_2, fstore_2);
            HANDLER_FOR(FSTORE_3, fstore_3);
            HANDLER_FOR(DSTORE_0, dstore_0);
            HANDLER_FOR(DSTORE_1, dstore_1);
            HANDLER_FOR(DSTORE_2, dstore_2);
            HANDLER_FOR(DSTORE_3, dstore_3);
            HANDLER_FOR(ASTORE_0, astore_0);
            HANDLER_FOR(ASTORE_1, astore_1);
            HANDLER_FOR(ASTORE_2, astore_2);
            HANDLER_FOR(ASTORE_3, astore_3);
            HANDLER_FOR(IASTORE, iastore);
            HANDLER_FOR(LASTORE, lastore);
            HANDLER_FOR(FASTORE, fastore);
            HANDLER_FOR(DASTORE, dastore);
            HANDLER_FOR(AASTORE, aastore);
            HANDLER_FOR(BASTORE, bastore);
            HANDLER_FOR(CASTORE, castore);
            HANDLER_FOR(SASTORE, sastore);
            HANDLER_FOR(POP, pop);
            HANDLER_FOR(POP2, pop2);
            HANDLER_FOR(DUP, dup);
            HANDLER_FOR(DUP_X1, dup_x1);
            HANDLER_FOR(DUP_X2, dup_x2);
            HANDLER_FOR(DUP2, dup2);
            HANDLER_FOR(DUP2_X1, dup2_x1);
            HANDLER_FOR(DUP2_X2, dup2_x2);
            HANDLER_FOR(SWAP, swap);
            default:
                ThrowException(vm, "java/lang/InternalError", "Unresolved instruction: %s - 0x%X", GetInstructionName(opcode), opcode);
                break;
        }
    }

    return NULL;
}
