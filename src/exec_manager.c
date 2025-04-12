#include <mem/exstack.h>
#include <stdlib.h>
#include <util/logging.h>
#include <nanojvm.h>
#include <classparse.h>

ThreadFrame *push_frame(VirtualMachine *vm, Method *method, ExStack *lvars, ExStack *opstack)
{    
    Thread *t = GetCurrent(vm);

    if (t == NULL) {
        error("Cannot push frame: Current thread is not registered in VM");
        return NULL;
    }

    t->frames = realloc(t->frames, sizeof(t->frame_count + 1));
    t->frames[t->frame_count] = (ThreadFrame) {
        .opstack = opstack,
        .locals = lvars,
        .method = method
    };
    t->frame_count++;
    return &t->frames[t->frame_count - 1];
}

ThreadFrame* pop_frame(VirtualMachine *vm) 
{
    Thread *t = GetCurrent(vm);

    if (t == NULL) {
        error("Cannot pop frame: Current thread is not registered in VM");
        return NULL;
    }

    if (t->frame_count == 0) {
        error("Cannot pop frame: Stack underflow (no frames left)");
        return NULL;
    }

    ThreadFrame *popped_frame = &t->frames[t->frame_count - 1];

    t->frame_count--;

    if (t->frame_count == 0) {
        free(t->frames);
        t->frames = NULL;
    } else {
        DestroyStack(popped_frame->locals);
        DestroyStack(popped_frame->opstack);
        ThreadFrame *new_frames = realloc(t->frames, t->frame_count * sizeof(ThreadFrame));
        if (new_frames) {
            t->frames = new_frames;
        } else {
            warn("pop_frame: realloc failed (leaking memory)");
        }
    }

    return popped_frame;
}

extern Item *execute_internal(VirtualMachine *vm, ThreadFrame *frame);

Item *ExecuteMethodBytecode(VirtualMachine *vm, Method *method, ExStack *lvars, ExStack *opstack)
{
    if (vm == NULL || method == NULL) return NULL;
    CodeAttribute *attr = &GetAttributeBySyntheticIdentifier(method->attributes, method->attribute_count, ATTR_CODE)->data.code;

    if (attr == NULL) {
        error("Method %s::%s has no code present on it", method->cf->name, method->name);
        return NULL;
    }

    if (opstack == NULL)
        opstack = CreateStack(attr->max_stack);
    else if (opstack->capacity != attr->max_stack) {
        error("Operand stack must have %d capacity(found %d)", attr->max_stack, opstack->capacity);
    }

    if (lvars == NULL)
        lvars = CreateStack(attr->max_locals);
    else if (lvars->capacity != attr->max_locals) {
        error("Local variable array must have %d capacity(found %d)", attr->max_locals, lvars->capacity);
    }

    ThreadFrame *frame = push_frame(vm, method, lvars, opstack);
    execute_internal(vm, frame);
    pop_frame(vm);

    return NULL;
}
