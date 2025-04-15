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

INSTRUCTION(new)
{
    uint16_t index = Read16();
    PushReference(opstack, Instantiate(vm, FindClass(vm, *frame->method->cf->constant_pool[index - 1].info._class.name)));
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
            HANDLER_FOR(NEW, new)
            case DUP:
                PushStack(opstack, Copy(opstack->data[opstack->top - 1]));
                break;
            case INVOKESPECIAL: {
                uint16_t index = Read16();
                MemberRef ref = cf->constant_pool[index - 1].info.member_ref;
                ClassFile *target = FindClass(vm, *ref.class_info->name);
                Method *m = GetMethodByNameAndDescriptor(target, *ref.name_and_type->name, *ref.name_and_type->descriptor);
                if (StringEquals(m->name, "<init>") && (vm->options->flags & OPTION_DISABLE_INIT) != 0) {
                    break;
                }
                PushStack(opstack, InvokeMethod(vm, m));
                break;
            }
            case ATHROW: {
                ObjectRegion *reg = PopReference(opstack);
                ThrowException(vm, reg->cf->name, "");
                break;
            }
            default:
                ThrowException(vm, "java/lang/InternalError", "Unresolved instruction: %s - 0x%X", GetInstructionName(opcode), opcode);
                break;
        }
    }

    return NULL;
}
