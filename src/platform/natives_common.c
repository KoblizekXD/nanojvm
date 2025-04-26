#ifndef __freestanding__

#include <platform/natives.h>
#include <stdlib.h>
#include <string.h>

char *GetNativeName(Method *method)
{
    size_t class_name_len = strlen(method->cf->name);
    size_t method_name_len = strlen(method->name);
    
    char *name = malloc(class_name_len + method_name_len + 3);
    if (name == NULL) {
        return NULL;
    }

    size_t i = 0;
    for (size_t j = 0; j < class_name_len; ++j) {
        if (method->cf->name[j] == '.' || method->cf->name[j] == '/') {
            name[i++] = '_';
        } else {
            name[i++] = method->cf->name[j];
        }
    }

    name[i++] = '_';
    name[i++] = '_';

    for (size_t j = 0; j < method_name_len; ++j) {
        if (method->name[j] == '.' || method->name[j] == '/') {
            name[i++] = '_';
        } else {
            name[i++] = method->name[j];
        }
    }

    name[i] = '\0';

    return name;
}

#else

char *GetNativeName(Method *method)
{
    return NULL;
}

#endif

void GetNativeNameEx(Method *method, char *name)
{
    size_t class_name_len = strlen(method->cf->name);
    size_t method_name_len = strlen(method->name);
    
    if (name == NULL) {
        return;
    }

    size_t i = 0;
    for (size_t j = 0; j < class_name_len; ++j) {
        if (method->cf->name[j] == '.' || method->cf->name[j] == '/') {
            name[i++] = '_';
        } else {
            name[i++] = method->cf->name[j];
        }
    }

    name[i++] = '_';
    name[i++] = '_';

    for (size_t j = 0; j < method_name_len; ++j) {
        if (method->name[j] == '.' || method->name[j] == '/') {
            name[i++] = '_';
        } else {
            name[i++] = method->name[j];
        }
    }

    name[i] = '\0';
}
