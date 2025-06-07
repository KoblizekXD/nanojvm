#include <nanojvm.h>
#include <memory/utils.h>

// Returns the offset to newly creatable frame
ThreadFrame *get_last_frame(Thread *thr)
{
    size_t off = 0;
    for (size_t i = 0; i < thr->frame_count; i++) {
        ThreadFrame *fr = (ThreadFrame*) ((uint8_t*) thr->frames + off);
        off += sizeof(ThreadFrame);
        off += (4 * fr->local_size) + (4 * fr->opstack_size);
    }
    return (ThreadFrame*) ((uint8_t*) thr->frames + off);
}

ThreadFrame *GetTopFrame(Thread *thr)
{
    if (thr->frame_count == 0) return NULL;
    size_t off = 0;
    for (size_t i = 0; i < thr->frame_count - 1; i++) {
        ThreadFrame *fr = (ThreadFrame*) ((uint8_t*) thr->frames + off);
        off += sizeof(ThreadFrame);
        off += (4 * fr->local_size) + (4 * fr->opstack_size);
    }
    return (ThreadFrame*) ((uint8_t*) thr->frames + off);
}

ThreadFrame *push_frame(FreestandingVirtualMachine *vm, Thread *thr, Method *method)
{
    thr->frame_count++;
    ThreadFrame *fr = get_last_frame(thr);
    fr->method = method;
    fr->pc = method->code->code;
    fr->opstack_top = 0;
    fr->opstack_size = method->code->max_stack;
    fr->local_size = method->code->max_locals;
    memset(fr->data, 0x0, (fr->local_size * 4) + (fr->opstack_size * 4));
    return fr;
}

void pop_frame(FreestandingVirtualMachine *vm, Thread *thr)
{
    ThreadFrame *fr = GetTopFrame(thr);
    memset(fr, 0, sizeof(ThreadFrame) + (4 * fr->local_size + 4 * fr->opstack_size));
    thr->frame_count--;
}

extern int ExecuteInternal(FreestandingVirtualMachine *vm, Thread *thread, ThreadFrame *top, Method *method); 

int Execute(FreestandingVirtualMachine *vm, Thread *thr, Method *method)
{
    ThreadFrame *frame = push_frame(vm, thr, method);
    int status = ExecuteInternal(vm, thr, frame, method);
    pop_frame(vm, thr);
    return status;
}
