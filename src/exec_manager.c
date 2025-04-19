#include <mem/exstack.h>
#include <stdlib.h>
#include <util/logging.h>
#include <nanojvm.h>
#include <classparse.h>
#include <platform/natives.h>

ThreadFrame *push_frame(VirtualMachine *vm, Method *method, ExStack *lvars, ExStack *opstack)
{    
    Thread *t = GetCurrent(vm);

    if (t == NULL) {
        error("Cannot push frame: Current thread is not registered in VM");
        return NULL;
    }

    t->frames = realloc(t->frames, sizeof(ThreadFrame) * (t->frame_count + 1));
    t->frames[t->frame_count] = (ThreadFrame) {
        .opstack = opstack,
        .locals = lvars,
        .method = method
    };
    t->frame_count++;
    return &t->frames[t->frame_count - 1];
}

void pop_frame(VirtualMachine *vm, Item *to_push) 
{
    Thread *t = GetCurrent(vm);

    if (t == NULL) {
        error("Cannot pop frame: Current thread is not registered in VM");
    }

    if (t->frame_count == 0) {
        error("Cannot pop frame: Stack underflow (no frames left)");
    }

    ThreadFrame *popped_frame = &t->frames[t->frame_count - 1];

    t->frame_count--;
    DestroyStack(popped_frame->locals);
    DestroyStack(popped_frame->opstack);
    free(popped_frame);

    if (t->frame_count == 0) {
        free(t->frames);
        t->frames = NULL;
    } else { 
        ThreadFrame *new_frames = realloc(t->frames, t->frame_count * sizeof(ThreadFrame));
        if (new_frames) {
            t->frames = new_frames;
            if (to_push != NULL) PushStack(t->frames[t->frame_count - 1].opstack, to_push);
        } else {
            warn("pop_frame: realloc failed (leaking memory)");
        }
    }
}

extern Item *execute_internal(VirtualMachine *vm, ThreadFrame *frame, ExStack *opstack, ExStack *lvars);

Item *ExecuteMethodBytecode(VirtualMachine *vm, Method *method, ExStack *lvars, ExStack *opstack)
{
    if (vm == NULL || method == NULL) return NULL;

    CodeAttribute *attr = method->code;

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
    Item *i = execute_internal(vm, frame, frame->opstack, frame->locals);
    
    if (i == NULL && GetReturnType(method) != 'V') {
        error("Unexpected return of void");
    }

    pop_frame(vm, i);

    return NULL;
}

void pass_parameters(ExStack *callee, ExStack *caller, size_t count)
{
    if (count < 1) return;
    for (size_t i = count; i > 0; i--) {
        callee->data[i - 1] = PopStack(caller);
    }
    callee->top += count;
}

Item *invoke_native_method(VirtualMachine *vm, Method *method)
{
    NativeFunction native = NULL;
    char *cname = GetNativeName(method);
    for (size_t i = 0; i < vm->natives_count; i++) {
        native = FindNativeFunction(vm->natives[i], cname);
        if (native) break;
    }

    if (!native) {
        error("Failed to find native method %s::%s(%s) in %d libraries", method->cf->name, method->name, cname, vm->natives_count);
        free(cname);
        return NULL;
    }
    free(cname);
    Thread *t = GetCurrent(vm);
    ExStack *stack = FrameCeiling(t).opstack;
    size_t argc = GetParameterCount(method);
    Item **argv = malloc(sizeof(Item*) * argc);
    for (size_t i = argc; i > 0; i--) {
        argv[i - 1] = PopStack(stack);
    }
    ObjectRegion *instance = NULL;
    if (!(method->access_flags & ACC_STATIC)) {
        instance = _pop_reference(stack);
    }
    return native(vm, instance, argc, argv);
}

Item *InvokeMethod(VirtualMachine *vm, Method *method)
{
    if (method->access_flags & ACC_NATIVE) {
        return invoke_native_method(vm, method);
    }

    CodeAttribute *attr = method->code;

    if (attr == NULL) {
        error("Method %s::%s has no code present on it", method->cf->name, method->name);
        return NULL;
    }

    Thread *t = GetCurrent(vm); 
    
    size_t argc = GetParameterCount(method);
    if (!(method->access_flags & ACC_STATIC)) {
        argc++;
    }

    ExStack *opstack = CreateStack(attr->max_stack);
    ExStack *locals = CreateStack(attr->max_locals);
    if (t->frame_count > 0) {
        ThreadFrame f = FrameCeiling(t);
        pass_parameters(locals, f.opstack, argc);
    } 
    return ExecuteMethodBytecode(vm, method, locals, opstack);
}
