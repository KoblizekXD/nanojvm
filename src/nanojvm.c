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

VirtualMachine *Initialize(const VmOptions *options)
{
    if (options == NULL)
        options = &DEFAULT_OPTIONS;

    VirtualMachine *vm = malloc(sizeof(VirtualMachine));
    vm->options = options;
    vm->loaded_classes_count = 0;
    vm->loaded_classes = NULL;
    vm->heap = InitializeHeap(options->heap_init);
    vm->jdk = SetupJDK();

    vm->thread_count = 1;
    vm->threads = malloc(sizeof(Thread));
    vm->threads->native_thread = thrd_current();
    vm->threads->frames = NULL;
    vm->threads->frame_count = 0;
    return vm;
}

void TearDown(VirtualMachine *vm)
{
    FreeHeap(vm->heap);
    for (size_t i = 0; i < vm->loaded_classes_count; i++) {
        FreeClassFile(vm->loaded_classes[i]);
    }
    free(vm->loaded_classes);
    mz_zip_reader_end(vm->jdk->handle);
    free(vm->jdk->handle);
    free(vm->jdk);
    for (size_t i = 0; i < vm->thread_count; i++) {
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

ClassFile *FindClass(VirtualMachine *vm, const char *name)
{
    ClassFile *cf = NULL;
    if (vm->jdk->mode != 0) cf = find_classfile_zip(vm->jdk->handle, name);
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
    // TODO: Linking

    if (cf == NULL) return NULL;
    vm->loaded_classes_count++;
    vm->loaded_classes = realloc(vm->loaded_classes, sizeof(ClassFile*) * vm->loaded_classes_count);
    vm->loaded_classes[vm->loaded_classes_count - 1] = cf;

    return cf;
}

Thread *GetCurrent(VirtualMachine *vm)
{
    thrd_t id = thrd_current();
    for (size_t i = 0; i < vm->thread_count; i++) {
        if (vm->threads[i].native_thread == id) return &vm->threads[i];
    }
    return NULL;
}
