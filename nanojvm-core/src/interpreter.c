#include <classfile.h>
#include <nanojvm.h>
#include <iterators.h>

typedef void (*instruction_handler)(
    ThreadFrameContext *ctx
);

#define INSTRUCTION_HANDLER(NAME) void instruction_##NAME( \
    ThreadFrameContext *ctx \
)

#define PASS_PARAMS ctx
#define PARAMS ThreadFrameContext *ctx

#define ReadByte(pc)   (*(pc) += 1, (*(pc))[-1])
#define ReadShort(pc)  (uint16_t)(*(pc) += 2, ((*(pc))[-2] << 8) | (*(pc))[-1])

INSTRUCTION_HANDLER(NOP)
{
    // No operation
}

INSTRUCTION_HANDLER(ACONST_NULL)
{

}

INSTRUCTION_HANDLER(ICONST_M1)
{
    PushInt(ctx->opstack, -1);
}

INSTRUCTION_HANDLER(ICONST_0)
{
    PushInt(ctx->opstack, 0);
}

INSTRUCTION_HANDLER(ICONST_1)
{
    PushInt(ctx->opstack, 1);
}

INSTRUCTION_HANDLER(ICONST_2)
{
    PushInt(ctx->opstack, 2);
}

INSTRUCTION_HANDLER(ICONST_3)
{
    PushInt(ctx->opstack, 3);
}

INSTRUCTION_HANDLER(ICONST_4)
{
    PushInt(ctx->opstack, 4);
}

INSTRUCTION_HANDLER(ICONST_5)
{
    PushInt(ctx->opstack, 5);
}

INSTRUCTION_HANDLER(LCONST_0)
{
    PushLong(ctx->opstack, 0);
}

INSTRUCTION_HANDLER(LCONST_1)
{
    PushLong(ctx->opstack, 1);
}

INSTRUCTION_HANDLER(FCONST_0)
{
    PushFloat(ctx->opstack, 0.0f);
}

INSTRUCTION_HANDLER(FCONST_1)
{
    PushFloat(ctx->opstack, 1.0f);
}

INSTRUCTION_HANDLER(FCONST_2)
{
    PushFloat(ctx->opstack, 2.0f);
}

INSTRUCTION_HANDLER(DCONST_0)
{
    PushDouble(ctx->opstack, 0.0);
}

INSTRUCTION_HANDLER(DCONST_1)
{
    PushDouble(ctx->opstack, 1.0);
}

INSTRUCTION_HANDLER(BIPUSH)
{
    PushInt(ctx->opstack, ReadByte(ctx->pc));
}

INSTRUCTION_HANDLER(SIPUSH)
{
    PushInt(ctx->opstack, (int16_t) ReadShort(ctx->pc));
}

INSTRUCTION_HANDLER(LDC)
{
}

INSTRUCTION_HANDLER(LDC_W)
{
}

INSTRUCTION_HANDLER(LDC2_W)
{
}

static void locals_to_opstack(PARAMS, const size_t size)
{
    const uint8_t index = ReadByte(ctx->pc);
    uint8_t value[size];
    MemoryCopy(&value, ctx->localvars->data + index * 4, size);
    ArrayStackPush(ctx->opstack, &value, size);
}

static void local_to_opstack(PARAMS, const size_t size, const size_t index)
{
    uint8_t value[size];
    MemoryCopy(&value, ctx->localvars->data + index * 4, size);
    ArrayStackPush(ctx->opstack, &value, size);
}

INSTRUCTION_HANDLER(ILOAD)
{
    locals_to_opstack(PASS_PARAMS, sizeof(int32_t));
}

INSTRUCTION_HANDLER(LLOAD)
{
    locals_to_opstack(PASS_PARAMS, sizeof(int64_t));
}

INSTRUCTION_HANDLER(FLOAD)
{
    locals_to_opstack(PASS_PARAMS, sizeof(float));
}

INSTRUCTION_HANDLER(DLOAD)
{
    locals_to_opstack(PASS_PARAMS, sizeof(double));
}

INSTRUCTION_HANDLER(ALOAD)
{
    locals_to_opstack(PASS_PARAMS, sizeof(VirtualAddress));
}

INSTRUCTION_HANDLER(ILOAD_0)
{
    local_to_opstack(PASS_PARAMS, sizeof(int32_t), 0);
}

INSTRUCTION_HANDLER(ILOAD_1)
{
    local_to_opstack(PASS_PARAMS, sizeof(int32_t), 1);
}

INSTRUCTION_HANDLER(ILOAD_2)
{
    local_to_opstack(PASS_PARAMS, sizeof(int32_t), 2);
}

INSTRUCTION_HANDLER(ILOAD_3)
{
    local_to_opstack(PASS_PARAMS, sizeof(int32_t), 3);
}

INSTRUCTION_HANDLER(LLOAD_0)
{
    local_to_opstack(PASS_PARAMS, sizeof(int64_t), 0);
}

INSTRUCTION_HANDLER(LLOAD_1)
{
    local_to_opstack(PASS_PARAMS, sizeof(int64_t), 1);
}

INSTRUCTION_HANDLER(LLOAD_2)
{
    local_to_opstack(PASS_PARAMS, sizeof(int64_t), 2);
}

INSTRUCTION_HANDLER(LLOAD_3)
{
    local_to_opstack(PASS_PARAMS, sizeof(int64_t), 3);
}

INSTRUCTION_HANDLER(FLOAD_0)
{
    local_to_opstack(PASS_PARAMS, sizeof(float), 0);
}

INSTRUCTION_HANDLER(FLOAD_1)
{
    local_to_opstack(PASS_PARAMS, sizeof(float), 1);
}

INSTRUCTION_HANDLER(FLOAD_2)
{
    local_to_opstack(PASS_PARAMS, sizeof(float), 2);
}

INSTRUCTION_HANDLER(FLOAD_3)
{
    local_to_opstack(PASS_PARAMS, sizeof(float), 3);
}

INSTRUCTION_HANDLER(DLOAD_0)
{
    local_to_opstack(PASS_PARAMS, sizeof(double), 0);
}

INSTRUCTION_HANDLER(DLOAD_1)
{
    local_to_opstack(PASS_PARAMS, sizeof(double), 1);
}

INSTRUCTION_HANDLER(DLOAD_2)
{
    local_to_opstack(PASS_PARAMS, sizeof(double), 2);
}

INSTRUCTION_HANDLER(DLOAD_3)
{
    local_to_opstack(PASS_PARAMS, sizeof(double), 3);
}

INSTRUCTION_HANDLER(ALOAD_0)
{
    local_to_opstack(PASS_PARAMS, sizeof(VirtualAddress), 0);
}

INSTRUCTION_HANDLER(ALOAD_1)
{
    local_to_opstack(PASS_PARAMS, sizeof(VirtualAddress), 1);
}

INSTRUCTION_HANDLER(ALOAD_2)
{
    local_to_opstack(PASS_PARAMS, sizeof(VirtualAddress), 2);
}

INSTRUCTION_HANDLER(ALOAD_3)
{
    local_to_opstack(PASS_PARAMS, sizeof(VirtualAddress), 3);
}

INSTRUCTION_HANDLER(IALOAD)
{
}

INSTRUCTION_HANDLER(LALOAD)
{
}

INSTRUCTION_HANDLER(FALOAD)
{
}

INSTRUCTION_HANDLER(DALOAD)
{
}

INSTRUCTION_HANDLER(AALOAD)
{
}

INSTRUCTION_HANDLER(BALOAD)
{
}

INSTRUCTION_HANDLER(CALOAD)
{
}

INSTRUCTION_HANDLER(SALOAD)
{
}

static void opstack_to_locals(PARAMS, const size_t size)
{
    const uint8_t index = ReadByte(ctx->pc);
    uint8_t value[size];
    ArrayStackPop(ctx->opstack, &value, size);
    MemoryCopy(ctx->localvars->data + index * 4, &value, size);
}

static void opstack_to_local(PARAMS, const size_t size, const size_t index)
{
    uint8_t value[size];
    ArrayStackPop(ctx->opstack, &value, size);
    MemoryCopy(ctx->localvars->data + index * 4, &value, size);
}

INSTRUCTION_HANDLER(ISTORE)
{
    opstack_to_locals(PASS_PARAMS, sizeof(int32_t));
}

INSTRUCTION_HANDLER(LSTORE)
{
    opstack_to_locals(PASS_PARAMS, sizeof(int64_t));
}

INSTRUCTION_HANDLER(FSTORE)
{
    opstack_to_locals(PASS_PARAMS, sizeof(float));
}

INSTRUCTION_HANDLER(DSTORE)
{
    opstack_to_locals(PASS_PARAMS, sizeof(double));
}

INSTRUCTION_HANDLER(ASTORE)
{
    opstack_to_locals(PASS_PARAMS, sizeof(VirtualAddress));
}

INSTRUCTION_HANDLER(ISTORE_0)
{
    opstack_to_local(PASS_PARAMS, sizeof(int32_t), 0);
}

INSTRUCTION_HANDLER(ISTORE_1)
{
    opstack_to_local(PASS_PARAMS, sizeof(int32_t), 1);
}

INSTRUCTION_HANDLER(ISTORE_2)
{
    opstack_to_local(PASS_PARAMS, sizeof(int32_t), 2);
}

INSTRUCTION_HANDLER(ISTORE_3)
{
    opstack_to_local(PASS_PARAMS, sizeof(int32_t), 3);
}

INSTRUCTION_HANDLER(LSTORE_0)
{
    opstack_to_local(PASS_PARAMS, sizeof(int64_t), 0);
}

INSTRUCTION_HANDLER(LSTORE_1)
{
    opstack_to_local(PASS_PARAMS, sizeof(int64_t), 1);
}

INSTRUCTION_HANDLER(LSTORE_2)
{
    opstack_to_local(PASS_PARAMS, sizeof(int64_t), 2);
}

INSTRUCTION_HANDLER(LSTORE_3)
{
    opstack_to_local(PASS_PARAMS, sizeof(int64_t), 3);
}

INSTRUCTION_HANDLER(FSTORE_0)
{
    opstack_to_local(PASS_PARAMS, sizeof(float), 0);
}

INSTRUCTION_HANDLER(FSTORE_1)
{
    opstack_to_local(PASS_PARAMS, sizeof(float), 1);
}

INSTRUCTION_HANDLER(FSTORE_2)
{
    opstack_to_local(PASS_PARAMS, sizeof(float), 2);
}

INSTRUCTION_HANDLER(FSTORE_3)
{
    opstack_to_local(PASS_PARAMS, sizeof(float), 3);
}

INSTRUCTION_HANDLER(DSTORE_0)
{
    opstack_to_local(PASS_PARAMS, sizeof(double), 0);
}

INSTRUCTION_HANDLER(DSTORE_1)
{
    opstack_to_local(PASS_PARAMS, sizeof(double), 1);
}

INSTRUCTION_HANDLER(DSTORE_2)
{
    opstack_to_local(PASS_PARAMS, sizeof(double), 2);
}

INSTRUCTION_HANDLER(DSTORE_3)
{
    opstack_to_local(PASS_PARAMS, sizeof(double), 3);
}

INSTRUCTION_HANDLER(ASTORE_0)
{
    opstack_to_local(PASS_PARAMS, sizeof(VirtualAddress), 0);
}

INSTRUCTION_HANDLER(ASTORE_1)
{
    opstack_to_local(PASS_PARAMS, sizeof(VirtualAddress), 1);
}

INSTRUCTION_HANDLER(ASTORE_2)
{
    opstack_to_local(PASS_PARAMS, sizeof(VirtualAddress), 2);
}

INSTRUCTION_HANDLER(ASTORE_3)
{
    opstack_to_local(PASS_PARAMS, sizeof(VirtualAddress), 3);
}

INSTRUCTION_HANDLER(IASTORE)
{
}

INSTRUCTION_HANDLER(LASTORE)
{
}

INSTRUCTION_HANDLER(FASTORE)
{
}

INSTRUCTION_HANDLER(DASTORE)
{
}

INSTRUCTION_HANDLER(AASTORE)
{
}

INSTRUCTION_HANDLER(BASTORE)
{
}

INSTRUCTION_HANDLER(CASTORE)
{
}

INSTRUCTION_HANDLER(SASTORE)
{
}

INSTRUCTION_HANDLER(POP)
{
    PopInt(ctx->opstack);
}

INSTRUCTION_HANDLER(POP2)
{
    PopLong(ctx->opstack);
}

INSTRUCTION_HANDLER(DUP)
{
    const int32_t value = PopInt(ctx->opstack);
    PushInt(ctx->opstack, value);
    PushInt(ctx->opstack, value);
}

INSTRUCTION_HANDLER(DUP_X1)
{
    const int32_t value = PopInt(ctx->opstack);
    const int32_t value2 = PopInt(ctx->opstack);
    PushInt(ctx->opstack, value);
    PushInt(ctx->opstack, value2);
    PushInt(ctx->opstack, value);
}

INSTRUCTION_HANDLER(DUP_X2)
{
    const int32_t value = PopInt(ctx->opstack);
    const int64_t value2 = PopLong(ctx->opstack);
    PushInt(ctx->opstack, value);
    PushLong(ctx->opstack, value2);
    PushInt(ctx->opstack, value);
}

INSTRUCTION_HANDLER(DUP2)
{
    const int64_t value = PopLong(ctx->opstack);
    PushLong(ctx->opstack, value);
    PushLong(ctx->opstack, value);
}

INSTRUCTION_HANDLER(DUP2_X1)
{
    const int64_t value = PopLong(ctx->opstack);
    const int32_t value2 = PopInt(ctx->opstack);
    PushLong(ctx->opstack, value);
    PushInt(ctx->opstack, value2);
    PushLong(ctx->opstack, value);
}

INSTRUCTION_HANDLER(DUP2_X2)
{
    const int64_t value = PopLong(ctx->opstack);
    const int64_t value2 = PopLong(ctx->opstack);
    PushLong(ctx->opstack, value);
    PushLong(ctx->opstack, value2);
    PushLong(ctx->opstack, value);
}

INSTRUCTION_HANDLER(SWAP)
{
    const int32_t value = PopInt(ctx->opstack);
    const int32_t value2 = PopInt(ctx->opstack);
    PushInt(ctx->opstack, value);
    PushInt(ctx->opstack, value2);
}

INSTRUCTION_HANDLER(IADD)
{
    const int32_t value = PopInt(ctx->opstack);
    const int32_t value2 = PopInt(ctx->opstack);
    PushInt(ctx->opstack, value + value2);
}

INSTRUCTION_HANDLER(LADD)
{
    const int64_t value = PopLong(ctx->opstack);
    const int64_t value2 = PopLong(ctx->opstack);
    PushLong(ctx->opstack, value + value2);
}

INSTRUCTION_HANDLER(FADD)
{
    const float value = PopFloat(ctx->opstack);
    const float value2 = PopFloat(ctx->opstack);
    PushFloat(ctx->opstack, value + value2);
}

INSTRUCTION_HANDLER(DADD)
{
    const double value = PopDouble(ctx->opstack);
    const double value2 = PopDouble(ctx->opstack);
    PushDouble(ctx->opstack, value + value2);
}

INSTRUCTION_HANDLER(ISUB)
{
    const int32_t value = PopInt(ctx->opstack);
    const int32_t value2 = PopInt(ctx->opstack);
    PushInt(ctx->opstack, value2 - value);
}

INSTRUCTION_HANDLER(LSUB)
{
    const int64_t value = PopLong(ctx->opstack);
    const int64_t value2 = PopLong(ctx->opstack);
    PushLong(ctx->opstack, value2 - value);
}

INSTRUCTION_HANDLER(FSUB)
{
    const float value = PopFloat(ctx->opstack);
    const float value2 = PopFloat(ctx->opstack);
    PushFloat(ctx->opstack, value - value2);
}

INSTRUCTION_HANDLER(DSUB)
{
    const double value = PopDouble(ctx->opstack);
    const double value2 = PopDouble(ctx->opstack);
    PushDouble(ctx->opstack, value2 - value);
}

INSTRUCTION_HANDLER(IMUL)
{
    PushInt(ctx->opstack, PopInt(ctx->opstack) * PopInt(ctx->opstack));
}

INSTRUCTION_HANDLER(LMUL)
{
    PushLong(ctx->opstack, PopLong(ctx->opstack) * PopLong(ctx->opstack));
}

INSTRUCTION_HANDLER(FMUL)
{
    PushFloat(ctx->opstack, PopFloat(ctx->opstack) * PopFloat(ctx->opstack));
}

INSTRUCTION_HANDLER(DMUL)
{
    PushDouble(ctx->opstack, PopDouble(ctx->opstack) * PopDouble(ctx->opstack));
}

INSTRUCTION_HANDLER(IDIV)
{
    const int32_t value2 = PopInt(ctx->opstack);
    const int32_t value1 = PopInt(ctx->opstack);
    PushInt(ctx->opstack, value1 / value2);
}

INSTRUCTION_HANDLER(LDIV)
{
    const int64_t value2 = PopLong(ctx->opstack);
    const int64_t value1 = PopLong(ctx->opstack);
    PushLong(ctx->opstack, value1 / value2);
}

INSTRUCTION_HANDLER(FDIV)
{
    const float value2 = PopFloat(ctx->opstack);
    const float value1 = PopFloat(ctx->opstack);
    PushFloat(ctx->opstack, value1 / value2);
}

INSTRUCTION_HANDLER(DDIV)
{
    const double value2 = PopDouble(ctx->opstack);
    const double value1 = PopDouble(ctx->opstack);
    PushDouble(ctx->opstack, value1 / value2);
}

INSTRUCTION_HANDLER(IREM)
{
    const int32_t value2 = PopInt(ctx->opstack);
    const int32_t value1 = PopInt(ctx->opstack);
    PushInt(ctx->opstack, value1 % value2);
}

INSTRUCTION_HANDLER(LREM)
{
    const int64_t value2 = PopLong(ctx->opstack);
    const int64_t value1 = PopLong(ctx->opstack);
    PushLong(ctx->opstack, value1 % value2);
}

INSTRUCTION_HANDLER(FREM)
{
    const float value2 = PopFloat(ctx->opstack);
    const float value1 = PopFloat(ctx->opstack);
    PushFloat(ctx->opstack, ModuloFloat(value1, value2));
}

INSTRUCTION_HANDLER(DREM)
{
}

INSTRUCTION_HANDLER(INEG)
{
}

INSTRUCTION_HANDLER(LNEG)
{
}

INSTRUCTION_HANDLER(FNEG)
{
}

INSTRUCTION_HANDLER(DNEG)
{
}

INSTRUCTION_HANDLER(ISHL)
{
}

INSTRUCTION_HANDLER(LSHL)
{
}

INSTRUCTION_HANDLER(ISHR)
{
}

INSTRUCTION_HANDLER(LSHR)
{
}

INSTRUCTION_HANDLER(IUSHR)
{
}

INSTRUCTION_HANDLER(LUSHR)
{
}

INSTRUCTION_HANDLER(IAND)
{
}

INSTRUCTION_HANDLER(LAND)
{
}

INSTRUCTION_HANDLER(IOR)
{
}

INSTRUCTION_HANDLER(LOR)
{
}

INSTRUCTION_HANDLER(IXOR)
{
}

INSTRUCTION_HANDLER(LXOR)
{
}

INSTRUCTION_HANDLER(IINC)
{
}

INSTRUCTION_HANDLER(I2L)
{
}

INSTRUCTION_HANDLER(I2F)
{
}

INSTRUCTION_HANDLER(I2D)
{
}

INSTRUCTION_HANDLER(L2I)
{
}

INSTRUCTION_HANDLER(L2F)
{
}

INSTRUCTION_HANDLER(L2D)
{
}

INSTRUCTION_HANDLER(F2I)
{
}

INSTRUCTION_HANDLER(F2L)
{
}

INSTRUCTION_HANDLER(F2D)
{
}

INSTRUCTION_HANDLER(D2I)
{
}

INSTRUCTION_HANDLER(D2L)
{
}

INSTRUCTION_HANDLER(D2F)
{
}

INSTRUCTION_HANDLER(I2B)
{
}

INSTRUCTION_HANDLER(I2C)
{
}

INSTRUCTION_HANDLER(I2S)
{
}

INSTRUCTION_HANDLER(LCMP)
{
}

INSTRUCTION_HANDLER(FCMPL)
{
}

INSTRUCTION_HANDLER(FCMPG)
{
}

INSTRUCTION_HANDLER(DCMPL)
{
}

INSTRUCTION_HANDLER(DCMPG)
{
}

INSTRUCTION_HANDLER(IFEQ)
{
}

INSTRUCTION_HANDLER(IFNE)
{
}

INSTRUCTION_HANDLER(IFLT)
{
}

INSTRUCTION_HANDLER(IFGE)
{
}

INSTRUCTION_HANDLER(IFGT)
{
}

INSTRUCTION_HANDLER(IFLE)
{
}

INSTRUCTION_HANDLER(IF_ICMPEQ)
{
}

INSTRUCTION_HANDLER(IF_ICMPNE)
{
}

INSTRUCTION_HANDLER(IF_ICMPLT)
{
}

INSTRUCTION_HANDLER(IF_ICMPGE)
{
}

INSTRUCTION_HANDLER(IF_ICMPGT)
{
}

INSTRUCTION_HANDLER(IF_ICMPLE)
{
}

INSTRUCTION_HANDLER(IF_ACMPEQ)
{
}

INSTRUCTION_HANDLER(IF_ACMPNE)
{
}

INSTRUCTION_HANDLER(GETSTATIC)
{
}

INSTRUCTION_HANDLER(PUTSTATIC)
{
}

INSTRUCTION_HANDLER(GETFIELD)
{
}

INSTRUCTION_HANDLER(PUTFIELD)
{
}

INSTRUCTION_HANDLER(INVOKEVIRTUAL)
{
}

INSTRUCTION_HANDLER(INVOKESPECIAL)
{
}

INSTRUCTION_HANDLER(INVOKESTATIC)
{
}

INSTRUCTION_HANDLER(INVOKEINTERFACE)
{
}

INSTRUCTION_HANDLER(INVOKEDYNAMIC)
{
}

INSTRUCTION_HANDLER(NEW)
{
}

INSTRUCTION_HANDLER(NEWARRAY)
{
}

INSTRUCTION_HANDLER(ANEWARRAY)
{
}

INSTRUCTION_HANDLER(ARRAYLENGTH)
{
}

INSTRUCTION_HANDLER(ATHROW)
{
}

INSTRUCTION_HANDLER(CHECKCAST)
{
}

INSTRUCTION_HANDLER(INSTANCEOF)
{
}

INSTRUCTION_HANDLER(MONITORENTER)
{
}

INSTRUCTION_HANDLER(MONITOREXIT)
{
}

INSTRUCTION_HANDLER(GOTO)
{
}

INSTRUCTION_HANDLER(JSR)
{
}

INSTRUCTION_HANDLER(RET)
{
}

INSTRUCTION_HANDLER(TABLESWITCH)
{
}

INSTRUCTION_HANDLER(LOOKUPSWITCH)
{
}

INSTRUCTION_HANDLER(IRETURN)
{
    if (ctx->prev) PushInt(ctx->prev->opstack, PopInt(ctx->opstack));
}

INSTRUCTION_HANDLER(LRETURN)
{
    if (ctx->prev) PushLong(ctx->prev->opstack, PopLong(ctx->opstack));
}

INSTRUCTION_HANDLER(FRETURN)
{
    if (ctx->prev) PushFloat(ctx->prev->opstack, PopFloat(ctx->opstack));
}

INSTRUCTION_HANDLER(DRETURN)
{
    if (ctx->prev) PushDouble(ctx->prev->opstack, PopDouble(ctx->opstack));
}

INSTRUCTION_HANDLER(ARETURN)
{
    if (ctx->prev) PushAddress(ctx->prev->opstack, PopAddress(ctx->opstack));
}

INSTRUCTION_HANDLER(RETURN)
{
    // Basically noop, but ends execution of the current method
}

INSTRUCTION_HANDLER(WIDE)
{
}

INSTRUCTION_HANDLER(MULTIANEWARRAY)
{
}

INSTRUCTION_HANDLER(IFNULL)
{
}

INSTRUCTION_HANDLER(IFNONNULL)
{
}

INSTRUCTION_HANDLER(GOTO_W)
{
}

INSTRUCTION_HANDLER(JSR_W)
{
}

INSTRUCTION_HANDLER(BREAKPOINT)
{
}

INSTRUCTION_HANDLER(IMPDEP1)
{
}

INSTRUCTION_HANDLER(IMPDEP2)
{
}

instruction_handler instruction_handlers[205] = {
    instruction_NOP,
    instruction_ACONST_NULL,
    instruction_ICONST_M1,
    instruction_ICONST_0,
    instruction_ICONST_1,
    instruction_ICONST_2,
    instruction_ICONST_3,
    instruction_ICONST_4,
    instruction_ICONST_5,
    instruction_LCONST_0,
    instruction_LCONST_1,
    instruction_FCONST_0,
    instruction_FCONST_1,
    instruction_FCONST_2,
    instruction_DCONST_0,
    instruction_DCONST_1,
    instruction_BIPUSH,
    instruction_SIPUSH,
    instruction_LDC,
    instruction_LDC_W,
    instruction_LDC2_W,
    instruction_ILOAD,
    instruction_LLOAD,
    instruction_FLOAD,
    instruction_DLOAD,
    instruction_ALOAD,
    instruction_ILOAD_0,
    instruction_ILOAD_1,
    instruction_ILOAD_2,
    instruction_ILOAD_3,
    instruction_LLOAD_0,
    instruction_LLOAD_1,
    instruction_LLOAD_2,
    instruction_LLOAD_3,
    instruction_FLOAD_0,
    instruction_FLOAD_1,
    instruction_FLOAD_2,
    instruction_FLOAD_3,
    instruction_DLOAD_0,
    instruction_DLOAD_1,
    instruction_DLOAD_2,
    instruction_DLOAD_3,
    instruction_ALOAD_0,
    instruction_ALOAD_1,
    instruction_ALOAD_2,
    instruction_ALOAD_3,
    instruction_IALOAD,
    instruction_LALOAD,
    instruction_FALOAD,
    instruction_DALOAD,
    instruction_AALOAD,
    instruction_BALOAD,
    instruction_CALOAD,
    instruction_SALOAD,
    instruction_ISTORE,
    instruction_LSTORE,
    instruction_FSTORE,
    instruction_DSTORE,
    instruction_ASTORE,
    instruction_ISTORE_0,
    instruction_ISTORE_1,
    instruction_ISTORE_2,
    instruction_ISTORE_3,
    instruction_LSTORE_0,
    instruction_LSTORE_1,
    instruction_LSTORE_2,
    instruction_LSTORE_3,
    instruction_FSTORE_0,
    instruction_FSTORE_1,
    instruction_FSTORE_2,
    instruction_FSTORE_3,
    instruction_DSTORE_0,
    instruction_DSTORE_1,
    instruction_DSTORE_2,
    instruction_DSTORE_3,
    instruction_ASTORE_0,
    instruction_ASTORE_1,
    instruction_ASTORE_2,
    instruction_ASTORE_3,
    instruction_IASTORE,
    instruction_LASTORE,
    instruction_FASTORE,
    instruction_DASTORE,
    instruction_AASTORE,
    instruction_BASTORE,
    instruction_CASTORE,
    instruction_SASTORE,
    instruction_POP,
    instruction_POP2,
    instruction_DUP,
    instruction_DUP_X1,
    instruction_DUP_X2,
    instruction_DUP2,
    instruction_DUP2_X1,
    instruction_DUP2_X2,
    instruction_SWAP,
    instruction_IADD,
    instruction_LADD,
    instruction_FADD,
    instruction_DADD,
    instruction_ISUB,
    instruction_LSUB,
    instruction_FSUB,
    instruction_DSUB,
    instruction_IMUL,
    instruction_LMUL,
    instruction_FMUL,
    instruction_DMUL,
    instruction_IDIV,
    instruction_LDIV,
    instruction_FDIV,
    instruction_DDIV,
    instruction_IREM,
    instruction_LREM,
    instruction_FREM,
    instruction_DREM,
    instruction_INEG,
    instruction_LNEG,
    instruction_FNEG,
    instruction_DNEG,
    instruction_ISHL,
    instruction_LSHL,
    instruction_ISHR,
    instruction_LSHR,
    instruction_IUSHR,
    instruction_LUSHR,
    instruction_IAND,
    instruction_LAND,
    instruction_IOR,
    instruction_LOR,
    instruction_IXOR,
    instruction_LXOR,
    instruction_IINC,
    instruction_I2L,
    instruction_I2F,
    instruction_I2D,
    instruction_L2I,
    instruction_L2F,
    instruction_L2D,
    instruction_F2I,
    instruction_F2L,
    instruction_F2D,
    instruction_D2I,
    instruction_D2L,
    instruction_D2F,
    instruction_I2B,
    instruction_I2C,
    instruction_I2S,
    instruction_LCMP,
    instruction_FCMPL,
    instruction_FCMPG,
    instruction_DCMPL,
    instruction_DCMPG,
    instruction_IFEQ,
    instruction_IFNE,
    instruction_IFLT,
    instruction_IFGE,
    instruction_IFGT,
    instruction_IFLE,
    instruction_IF_ICMPEQ,
    instruction_IF_ICMPNE,
    instruction_IF_ICMPLT,
    instruction_IF_ICMPGE,
    instruction_IF_ICMPGT,
    instruction_IF_ICMPLE,
    instruction_IF_ACMPEQ,
    instruction_IF_ACMPNE,
    instruction_GETSTATIC,
    instruction_PUTSTATIC,
    instruction_GETFIELD,
    instruction_PUTFIELD,
    instruction_INVOKEVIRTUAL,
    instruction_INVOKESPECIAL,
    instruction_INVOKESTATIC,
    instruction_INVOKEINTERFACE,
    instruction_INVOKEDYNAMIC,
    instruction_NEW,
    instruction_NEWARRAY,
    instruction_ANEWARRAY,
    instruction_ARRAYLENGTH,
    instruction_ATHROW,
    instruction_CHECKCAST,
    instruction_INSTANCEOF,
    instruction_MONITORENTER,
    instruction_MONITOREXIT,
    instruction_GOTO,
    instruction_JSR,
    instruction_RET,
    instruction_TABLESWITCH,
    instruction_LOOKUPSWITCH,
    instruction_IRETURN,
    instruction_LRETURN,
    instruction_FRETURN,
    instruction_DRETURN,
    instruction_ARETURN,
    instruction_RETURN,
    instruction_WIDE,
    instruction_MULTIANEWARRAY,
    instruction_IFNULL,
    instruction_IFNONNULL,
    instruction_GOTO_W,
    instruction_JSR_W,
    instruction_BREAKPOINT,
    instruction_IMPDEP1,
    instruction_IMPDEP2
};

int ThreadCurrentExecute(const VirtualMachine *vm, const ClassFile *classfile, Method *method, ThreadFrameContext *ctx)
{
    const CodeAttribute code = GetCodeAttribute(classfile, method->code_attr_off);
    CreateArrayStack(opstack, code.max_stack * sizeof(uint32_t));
    CreateArrayStack(localvars, code.max_locals * sizeof(uint32_t));
    uint8_t *pc = code.code;

    ThreadFrameContext frame = (ThreadFrameContext) {
        .vm = vm,
        .cf = classfile,
        .method = method,
        .opstack = &opstack,
        .localvars = &localvars,
        .pc = &pc,
        .code = &code,
        .prev = ctx,
        .exception = (ContextExceptionFlag) {0}
    };

    while (pc < code.code + code.code_length) {
        const uint8_t opcode = *pc;
        (*pc)++;
        if (opcode < 0xFE) {
            instruction_handlers[opcode](&frame);
            if (opcode >= 172 && opcode <= 177) break;
        } else if (opcode == 0xFE)
            instruction_IMPDEP1(&frame);
        else instruction_IMPDEP2(&frame);

        if (frame.exception.class_name != NULL) {
            for (int i = 0; i < code.exception_table_length; ++i) {
                const ExceptionTableEntry entry = code.exception_table[i];
                if ((pc - code.code) - 1 >= entry.start_pc && (pc - code.code) - 1 < entry.end_pc) {
                    if (entry.catch_type == 0) { // finally block
                        pc = code.code + entry.handler_pc;
                        frame.exception = (ContextExceptionFlag) {0};
                        break;
                    }
                    ConstantPoolEntry cp_info = GetConstantPoolEntry(classfile, entry.catch_type - 1);
                    if (cp_info.tag != CONSTANT_Class) continue;
                    cp_info = GetConstantPoolEntry(classfile, cp_info.info.class_info.name_index - 1);
                    if (cp_info.tag != CONSTANT_Utf8) continue;
                    const struct CONSTANT_Utf8_info class_name = cp_info.info.utf8_info;
                    if (class_name.length == frame.exception.class_name->length
                        && MemoryCompare(class_name.bytes, frame.exception.class_name->bytes, class_name.length) == 0) {
                        pc = code.code + entry.handler_pc;
                        const VirtualAddress exception_obj = PopAddress(frame.opstack);
                        ArrayStackClear(frame.opstack);
                        PushAddress(frame.opstack, exception_obj);
                        frame.exception = (ContextExceptionFlag) {0};
                        break;
                    }
                }
            }
            if (frame.exception.class_name != NULL) { // if exception not handled, propagate it to the previous frame
                if (ctx) {
                    ctx->exception = frame.exception;
                    return 0;
                }
                return 1;
            }
        }
    }
    return 0;
}
