#include "objects.h"
#include <mem/exstack.h>
#include <mem/memutils.h>
#include <stddef.h>
#include <threads.h>
#include <util/logging.h>
#include <util/strings.h>
#include <errno.h>
#include <classparse.h>
#include <mem/heap.h>
#include <nanojvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <miniz.h>
#include <string.h>
#include <vmopts.h>

VirtualMachine *Initialize(VmOptions *options)
{
    if (options == NULL)
        options = GetDefaultOptions();

    VirtualMachine *vm = malloc(sizeof(VirtualMachine));
    vm->options = options;
    vm->loaded_classes_count = 0;
    vm->loaded_classes = NULL;
    vm->heap = InitializeHeap(options->heap_init);
    if (!(vm->options->flags & OPTION_DISABLE_JVM_LOOKUP))
        vm->jdk = SetupJDK();
    else vm->jdk = NULL;
    vm->thread_count = 1;
    vm->threads = malloc(sizeof(Thread));
    vm->threads->native_thread = thrd_current();
    vm->threads->frames = NULL;
    vm->threads->frame_count = 0;
    vm->string_count = 0;
    vm->string_pool = NULL;
    return vm;
}

void TearDown(VirtualMachine *vm)
{
    MemoryInformation info = GetMemoryState();
    debug("Memory state before exit: Using %llu bytes, %llu bytes heap allocated", info.total_usage, info.heap_allocated);
    FreeOptionsIfPossible(vm->options);
    FreeHeap(vm->heap);
    for (size_t i = 0; i < vm->loaded_classes_count; i++) {
        ClassFile *cf = vm->loaded_classes[i];
        for (size_t j = 0; j < cf->field_count; j++) {
            if (cf->fields[j].value) free(cf->fields[j].value);
        }
        FreeClassFile(vm->loaded_classes[i]);
    }
    free(vm->loaded_classes);
    if (vm->jdk) {
        mz_zip_reader_end(vm->jdk->handle);
        free(vm->jdk->handle);
        free(vm->jdk);
    }
    for (size_t i = 0; i < vm->thread_count; i++) {
        for (size_t j = 0; j < vm->threads[i].frame_count; j++) {
            DestroyStack(vm->threads[i].frames[j].opstack);
            DestroyStack(vm->threads[i].frames[j].locals);
        }
        free(vm->threads[i].frames);
    }
    free(vm->threads);
}

ClassFile *find_classfile_zip(mz_zip_archive *archive, const char *classname)
{
    if (archive == NULL) return NULL;
    char *combined = malloc(strlen(classname) + 7);
    strcpy(combined, classname);
    strcat(combined, ".class");
    unsigned int num_files = mz_zip_reader_get_num_files(archive);
    for (size_t i = 0; i < num_files; i++) {
        unsigned int req = mz_zip_reader_get_filename(archive, i, NULL, 0);
        char *name = malloc(req);
        if (!mz_zip_reader_get_filename(archive, i, name, req)) {
            free(name);
            continue;
        } 
        if (EndsWith(name, combined)) {
            size_t file_size;
            void *data = mz_zip_reader_extract_to_heap(archive, i, &file_size, 0);
            free(name);
            free(combined);
            ClassFile *cf = ReadFrom(data);
            if (strcmp(cf->name, classname) != 0) {
                FreeClassFile(cf);
                mz_free(data);
                continue;
            }
            mz_free(data);
            return cf;
        }
        free(name);
    }
    free(combined);
    return NULL;
}

void link_class(VirtualMachine *vm, ClassFile *cf)
{
    debug("Link %s", cf->name);
    FindClass(vm, cf->super_name);
    for (size_t i = 0; i < cf->interface_count; i++) {
        FindClass(vm, cf->interfaces[i]);
    }

    for (size_t i = 0; i < cf->field_count; i++) {
        Field f = cf->fields[i];
        AttributeInfo *attr = GetAttributeBySyntheticIdentifier(f.attributes, f.attribute_count, ATTR_CONSTANT_VALUE);
        if (!attr || !(f.access_flags & ACC_STATIC)) continue;
        ConstantPoolEntry *entry = attr->data.constant_value.value;
        switch (entry->tag) {
            case CONSTANT_Float:
                cf->fields[i].value = CreateItem(STACK_ELEMENT_FLOATING | STACK_ELEMENT_INT, &entry->info.int_float);
                break;
            case CONSTANT_Integer:
                cf->fields[i].value = CreateItem(STACK_ELEMENT_INT, &entry->info.int_float);
                break;
            case CONSTANT_Double: {
                long l = (((long) entry->info.long_double.high_bytes << 32) + entry->info.long_double.low_bytes);
                cf->fields[i].value = CreateItem(STACK_ELEMENT_FLOATING | STACK_ELEMENT_LONG, &l);
                break;
            }
            case CONSTANT_Long: {
                long l = (((long)((uint64_t)entry->info.long_double.high_bytes << 32)) | entry->info.long_double.low_bytes);
                cf->fields[i].value = CreateItem(STACK_ELEMENT_LONG, &l);
                break;
            }
            case CONSTANT_String: {
                ObjectRegion *ref = InstantiateString(vm, *entry->info.string.string);
                cf->fields[i].value = CreateItem(STACK_ELEMENT_IS_ADDRESS | STACK_ELEMENT_LONG, &ref);
                break;
            }
        }
    }

    Method *static_init = GetMethodByName(cf, "<clinit>");

    if (static_init && (vm->options->flags & OPTION_DISABLE_CLINIT) == 0) {
        ExecuteMethodBytecode(vm, static_init, NULL, NULL);
    }
}

ClassFile *LoadExternal(VirtualMachine *vm, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    ClassFile *cf = ReadFromStream(f);
    if (!cf) {
        fclose(f);
        return NULL;
    }
    vm->loaded_classes_count++;
    vm->loaded_classes = realloc(vm->loaded_classes, sizeof(ClassFile*) * vm->loaded_classes_count);
    vm->loaded_classes[vm->loaded_classes_count - 1] = cf;
    fclose(f);

    link_class(vm, cf);

    return cf;
}

ClassFile *FindClass(VirtualMachine *vm, const char *name)
{
    if (name == NULL) return NULL;
    if (vm->loaded_classes) {
        for (size_t i = 0; i < vm->loaded_classes_count; i++) {
            if (StringEquals(vm->loaded_classes[i]->name, name)) return vm->loaded_classes[i];
        }
    }
    ClassFile *cf = NULL;
    if (vm->jdk && vm->jdk->mode != 0) cf = find_classfile_zip(vm->jdk->handle, name);
    for (size_t i = 0; i < vm->options->classpath_len; i++) {
        const char *str = vm->options->classpath[i];
        if (EndsWith(str, ".class")) {
            FILE *stream = fopen(str, "rb");
            if (stream == NULL) {
                warn("Cannot open stream for %s: %s", str, strerror(errno));
                continue;
            }
            char *strname = PeekClassName(stream);
            if (StringEquals(strname, name)) { 
                cf = ReadFromStream(stream);
                free(strname);
                fclose(stream);
                break;
            }
            free(strname);
            fclose(stream);
        } // TODO: Add support for other .jar(s)
    }

    if (cf == NULL) {
        error("ClassNotFound: %s", name);
        return NULL;
    }
    vm->loaded_classes_count++;
    vm->loaded_classes = realloc(vm->loaded_classes, sizeof(ClassFile*) * vm->loaded_classes_count);
    vm->loaded_classes[vm->loaded_classes_count - 1] = cf;

    link_class(vm, cf);

    // Guessing we could also eagerly load native functions? TODO

    return cf;
}

void ForceLoad(VirtualMachine *vm, ClassFile *cf)
{
    if (cf == NULL) return;
    vm->loaded_classes_count++;
    vm->loaded_classes = realloc(vm->loaded_classes, sizeof(ClassFile*) * vm->loaded_classes_count);
    vm->loaded_classes[vm->loaded_classes_count - 1] = cf;
}

Thread *GetCurrent(VirtualMachine *vm)
{
    thrd_t id = thrd_current();
    for (size_t i = 0; i < vm->thread_count; i++) {
        if (vm->threads[i].native_thread == id) return &vm->threads[i];
    }
    return NULL;
}
