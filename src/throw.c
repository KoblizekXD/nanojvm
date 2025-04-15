#include "objects.h"
#include <classparse.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <limits.h>
#include <nanojvm.h>
#include <stdio.h>
#include <threads.h>
#include <util/strings.h>
#include <string.h>

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
    Thread *t = GetCurrent(vm);

    int to_pop = 0;
    for (int i = t->frame_count - 1; i >= 0; i--) {
        ThreadFrame frame = t->frames[i];
        struct _exc_table *table = frame.method->code->exception_table;
        int valid = 0;
        for (size_t j = 0; j < frame.method->code->exception_table_length; j++) {
            if (Extends(vm, FindClass(vm, type), FindClass(vm, *table[j].catch_type->name)) && frame.pc >= table[j].start_pc && frame.pc - 1 < table[j].end_pc) {
                valid = 1;
                frame.pc = table[j].handler_pc;
                break; 
            }
        }
        if (!valid) to_pop++;
        else break;
    }

    if ((size_t) to_pop != t->frame_count) {
        for (int i = 0; i < to_pop; i++) {
            ThreadFrame *popped_frame = &FrameCeiling(t);
            t->frame_count--;
            DestroyStack(popped_frame->locals);
            DestroyStack(popped_frame->opstack);
            t->frames = realloc(t->frames, t->frame_count * sizeof(ThreadFrame));
        }
        ThreadFrame *frame = &FrameCeiling(t); 
        longjmp(frame->ret_buf, 1);
    }

    va_list args;
    va_start(args, message);

    fprintf(stderr, "\x1b[31m%s: ", type);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\x1b[0m\n");

    va_end(args);
    print_stack_trace(GetCurrent(vm));

    thrd_exit(1);
}
