#ifndef NANOJVM_PLATFORM_NATIVES_H
#define NANOJVM_PLATFORM_NATIVES_H

#include <classparse.h>
#include <nanojvm.h>
#include <objects.h>
#include <mem/exstack.h>
#include <stddef.h>

typedef Item *(*NativeFunction)(VirtualMachine *vm, ObjectRegion *instance, size_t argc, Item **argv);

// Returns a dynamically malloc'd lookup name for native method METHOD
char *GetNativeName(Method *method);
void GetNativeNameEx(Method *method, char *buffer); // The buffer size will always be strlen(method->name) + 3

void *OpenLibrary(const char *path);
void CloseLibrary(void *handle);
NativeFunction FindNativeFunction(void *library, const char *name);
void *RequestSystemResources(size_t count);

#endif
