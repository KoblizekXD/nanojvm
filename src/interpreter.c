#include <classparse.h>
#include <nanojvm.h>
#include <memory/opstack.h>

#define Read16() (*top->pc << 8) | *(top->pc + 1); top->pc += 2
#define Read8() *top->pc++

#define INSTRUCTION_PARAMS FreestandingVirtualMachine *vm, Thread *thread, ThreadFrame *frame
#define PASS_PARAMS vm, thread, top 
#define INSTRUCTION(NAME) static inline void handler_##NAME(FreestandingVirtualMachine *vm, Thread *thread, ThreadFrame *frame)
#define HANDLER_FOR(INSN, HANDLER) case INSN: handler_##HANDLER(vm, thread, top); break;

extern ThreadFrame *push_frame(FreestandingVirtualMachine *vm, Thread *thr, Method *method);
extern void pop_frame(FreestandingVirtualMachine *vm, Thread *thr);

int ExecuteInternal(FreestandingVirtualMachine *vm, Thread *thread, ThreadFrame *top, Method *method)
{
    while (top && top->pc) {
        uint8_t opcode = Read8();
        switch (opcode) {
            case NOP: break;
            case INVOKEVIRTUAL: {
                uint16_t index = Read16();
                MemberRef ref = top->method->cf->constant_pool[index - 1].info.member_ref;
                Method *m = GetMethodByNameAndDescriptor(FindClass(vm, *ref.class_info->name), *ref.name_and_type->name, *ref.name_and_type->descriptor);
                top = push_frame(vm, thread, m);
                // TODO: Copy over the parameters
                break;
            }
            case RETURN:
                pop_frame(vm, thread);
                top = GetTopFrame(thread); 
                break;
            case IRETURN: {
                int32_t i = PopInt(top);
                pop_frame(vm, thread);
                top = GetTopFrame(thread);
                PushInt(top, i);
                break;
            }
            case ARETURN: {
                void *ref = PopReference(top);
                pop_frame(vm, thread);
                top = GetTopFrame(thread);
                PushReference(top, ref);
                break;
            }
            case LRETURN: {
                int64_t l = PopLong(top);
                pop_frame(vm, thread);
                top = GetTopFrame(thread);
                PushLong(top, l);
                break;
            }
            case FRETURN: {
                float f = PopFloat(top);
                pop_frame(vm, thread);
                top = GetTopFrame(thread);
                PushFloat(top, f);
                break;
            }
            case DRETURN: {
                double d = PopDouble(top);
                pop_frame(vm, thread);
                top = GetTopFrame(thread);
                PushDouble(top, d);
                break;
            }

        }
    }
    return 0;
}
