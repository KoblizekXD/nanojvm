#include <classparse.h>
#include <stdarg.h>
#include <limits.h>
#include <nanojvm.h>
#include <stdio.h>

int get_closest_line(ThreadFrame *frame)
{
    AttributeInfo *info = GetAttributeBySyntheticIdentifier(frame->method->code->attributes, frame->method->code->attributes_count, ATTR_LINE_NUMBER_TABLE);
    if (!info) return -1;
    LineNumberTableAttribute lnt = info->data.line_number_table;

    int closest_line = -1;
    uint8_t *closest_pc = 0;
    for (size_t i = 0; i < lnt.line_number_table_length; i++) {
        struct line_number_table s = lnt.line_number_table[i];
        if (frame->pc >= s.start_pc) {
            if (closest_line == -1 || s.start_pc >= closest_pc) {
                closest_pc = s.start_pc;
                closest_line = s.line_number;
            }
        }
    }
    return closest_line;
}

void print_stack_trace(Thread *thread)
{
    for (int i = thread->frame_count - 1; i >= 0; i--) {
        ThreadFrame frame = thread->frames[i];
        AttributeInfo *src_file = GetAttributeBySyntheticIdentifier(frame.method->cf->attributes, frame.method->cf->attribute_count, ATTR_SOURCE_FILE);
        if (src_file) {
            fprintf(stderr, "\t\x1b[31mat %s::%s(%s:%d)\x1b[0m\n", frame.method->cf->name, frame.method->name, *src_file->data.sourcefile.sourcefile, get_closest_line(&frame));
        } 
        else fprintf(stderr, "\t\x1b[31mat %s::%s(line=%d, pc=0x%p)\x1b[0m\n", frame.method->cf->name, frame.method->name, get_closest_line(&frame), (void*) frame.pc);
    }
}

void ThrowException(VirtualMachine *vm, const char *type, const char *message, ...)
{
    va_list args;
    va_start(args, message);

    fprintf(stderr, "\x1b[31m%s: ", type);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\x1b[0m\n");

    va_end(args);
    print_stack_trace(GetCurrent(vm));
}
