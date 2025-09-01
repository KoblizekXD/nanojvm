#include <nanojvm.h>

extern int bytecode_execute_internal(
    const VirtualMachine *vm,
    const ClassFile *cf,
    Method *method,
    ArrayStack *opstack,
    ArrayStack *localvars,
    uint8_t **pc,
    const CodeAttribute *code
);

int ThreadCurrentExecute(const VirtualMachine *vm, const ClassFile *classfile, Method *method)
{
    const CodeAttribute code = GetCodeAttribute(classfile, method->code_attr_off);
    CreateArrayStack(opstack, code.max_stack * sizeof(uint32_t));
    CreateArrayStack(localvars, code.max_locals * sizeof(uint32_t));
    uint8_t *pc = code.code;
    return bytecode_execute_internal(vm, classfile, method, &opstack, &localvars, &pc, &code);
}