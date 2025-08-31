#include <classfile.h>
#include <nanojvm.h>

extern int frame_execute(const ClassFile *cf, Method *method);

typedef void (*instruction_handler)(
    const VirtualMachine *vm,
    const ClassFile *cf,
    Method *method,
    ArrayStack *opstack,
    ArrayStack *localvars,
    uint8_t **pc,
    const CodeAttribute *code
);

#define INSTRUCTION_HANDLER(NAME) void *instruction_##NAME( \
    const VirtualMachine *vm, \
    const ClassFile *cf, \
    Method *method, \
    ArrayStack *opstack, \
    ArrayStack *localvars, \
    uint8_t **pc, \
    const CodeAttribute *code \
)

INSTRUCTION_HANDLER(NOP)
{
}

INSTRUCTION_HANDLER(ACONST_NULL)
{
}

INSTRUCTION_HANDLER(ICONST_M1)
{
}

INSTRUCTION_HANDLER(ICONST_0)
{
}

INSTRUCTION_HANDLER(ICONST_1)
{
}

INSTRUCTION_HANDLER(ICONST_2)
{
}

INSTRUCTION_HANDLER(ICONST_3)
{
}

INSTRUCTION_HANDLER(ICONST_4)
{
}

INSTRUCTION_HANDLER(ICONST_5)
{
}

INSTRUCTION_HANDLER(LCONST_0)
{
}

INSTRUCTION_HANDLER(LCONST_1)
{
}

INSTRUCTION_HANDLER(FCONST_0)
{
}

INSTRUCTION_HANDLER(FCONST_1)
{
}

INSTRUCTION_HANDLER(FCONST_2)
{
}

INSTRUCTION_HANDLER(DCONST_0)
{
}

INSTRUCTION_HANDLER(DCONST_1)
{
}

INSTRUCTION_HANDLER(BIPUSH)
{
}

INSTRUCTION_HANDLER(SIPUSH)
{
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

INSTRUCTION_HANDLER(ILOAD)
{
}

INSTRUCTION_HANDLER(LLOAD)
{
}

INSTRUCTION_HANDLER(FLOAD)
{
}

INSTRUCTION_HANDLER(DLOAD)
{
}

INSTRUCTION_HANDLER(ALOAD)
{
}

INSTRUCTION_HANDLER(ILOAD_0)
{
}

INSTRUCTION_HANDLER(ILOAD_1)
{
}

INSTRUCTION_HANDLER(ILOAD_2)
{
}

INSTRUCTION_HANDLER(ILOAD_3)
{
}

INSTRUCTION_HANDLER(LLOAD_0)
{
}

INSTRUCTION_HANDLER(LLOAD_1)
{
}

INSTRUCTION_HANDLER(LLOAD_2)
{
}

INSTRUCTION_HANDLER(LLOAD_3)
{
}

INSTRUCTION_HANDLER(FLOAD_0)
{
}

INSTRUCTION_HANDLER(FLOAD_1)
{
}

INSTRUCTION_HANDLER(FLOAD_2)
{
}

INSTRUCTION_HANDLER(FLOAD_3)
{
}

INSTRUCTION_HANDLER(DLOAD_0)
{
}

INSTRUCTION_HANDLER(DLOAD_1)
{
}

INSTRUCTION_HANDLER(DLOAD_2)
{
}

INSTRUCTION_HANDLER(DLOAD_3)
{
}

INSTRUCTION_HANDLER(ALOAD_0)
{
}

INSTRUCTION_HANDLER(ALOAD_1)
{
}

INSTRUCTION_HANDLER(ALOAD_2)
{
}

INSTRUCTION_HANDLER(ALOAD_3)
{
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

INSTRUCTION_HANDLER(ISTORE)
{
}

INSTRUCTION_HANDLER(LSTORE)
{
}

INSTRUCTION_HANDLER(FSTORE)
{
}

INSTRUCTION_HANDLER(DSTORE)
{
}

INSTRUCTION_HANDLER(ASTORE)
{
}

INSTRUCTION_HANDLER(ISTORE_0)
{
}

INSTRUCTION_HANDLER(ISTORE_1)
{
}

INSTRUCTION_HANDLER(ISTORE_2)
{
}

INSTRUCTION_HANDLER(ISTORE_3)
{
}

INSTRUCTION_HANDLER(LSTORE_0)
{
}

INSTRUCTION_HANDLER(LSTORE_1)
{
}

INSTRUCTION_HANDLER(LSTORE_2)
{
}

INSTRUCTION_HANDLER(LSTORE_3)
{
}

INSTRUCTION_HANDLER(FSTORE_0)
{
}

INSTRUCTION_HANDLER(FSTORE_1)
{
}

INSTRUCTION_HANDLER(FSTORE_2)
{
}

INSTRUCTION_HANDLER(FSTORE_3)
{
}

INSTRUCTION_HANDLER(DSTORE_0)
{
}

INSTRUCTION_HANDLER(DSTORE_1)
{
}

INSTRUCTION_HANDLER(DSTORE_2)
{
}

INSTRUCTION_HANDLER(DSTORE_3)
{
}

INSTRUCTION_HANDLER(ASTORE_0)
{
}

INSTRUCTION_HANDLER(ASTORE_1)
{
}

INSTRUCTION_HANDLER(ASTORE_2)
{
}

INSTRUCTION_HANDLER(ASTORE_3)
{
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
}

INSTRUCTION_HANDLER(POP2)
{
}

INSTRUCTION_HANDLER(DUP)
{
}

INSTRUCTION_HANDLER(DUP_X1)
{
}

INSTRUCTION_HANDLER(DUP_X2)
{
}

INSTRUCTION_HANDLER(DUP2)
{
}

INSTRUCTION_HANDLER(DUP2_X1)
{
}

INSTRUCTION_HANDLER(DUP2_X2)
{
}

INSTRUCTION_HANDLER(SWAP)
{
}

INSTRUCTION_HANDLER(IADD)
{
}

INSTRUCTION_HANDLER(LADD)
{
}

INSTRUCTION_HANDLER(FADD)
{
}

INSTRUCTION_HANDLER(DADD)
{
}

INSTRUCTION_HANDLER(ISUB)
{
}

INSTRUCTION_HANDLER(LSUB)
{
}

INSTRUCTION_HANDLER(FSUB)
{
}

INSTRUCTION_HANDLER(DSUB)
{
}

INSTRUCTION_HANDLER(IMUL)
{
}

INSTRUCTION_HANDLER(LMUL)
{
}

INSTRUCTION_HANDLER(FMUL)
{
}

INSTRUCTION_HANDLER(DMUL)
{
}

INSTRUCTION_HANDLER(IDIV)
{
}

INSTRUCTION_HANDLER(LDIV)
{
}

INSTRUCTION_HANDLER(FDIV)
{
}

INSTRUCTION_HANDLER(DDIV)
{
}

INSTRUCTION_HANDLER(IREM)
{
}

INSTRUCTION_HANDLER(LREM)
{
}

INSTRUCTION_HANDLER(FREM)
{
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
}

INSTRUCTION_HANDLER(LRETURN)
{
}

INSTRUCTION_HANDLER(FRETURN)
{
}

INSTRUCTION_HANDLER(DRETURN)
{
}

INSTRUCTION_HANDLER(ARETURN)
{
}

INSTRUCTION_HANDLER(RETURN)
{
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

int bytecode_execute_internal(
    const VirtualMachine *vm,
    const ClassFile *cf,
    Method *method,
    ArrayStack *opstack,
    ArrayStack *localvars,
    uint8_t **pc,
    const CodeAttribute *code
)
{
    while (*pc < code->code + code->code_length) {
        if (**pc == 0xFF)
            instruction_IMPDEP2(vm, cf, method, opstack, localvars, pc, code);
        else if (**pc == 0xFE)
            instruction_IMPDEP1(vm, cf, method, opstack, localvars, pc, code);
        else instruction_handlers[**pc](vm, cf, method, opstack, localvars, pc, code);
    }
    return 0;
}