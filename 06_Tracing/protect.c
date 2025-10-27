#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

int (*original_remove)(const char* filename) = NULL;

int remove(const char* filename) {
    if (original_remove == NULL) {
        original_remove = dlsym(RTLD_NEXT, "remove");
    }
    
    if (filename != NULL && strstr(filename, "PROTECT") != NULL) {
        fprintf(stderr, "protected: File '%s' is protected and can't be deleted\n", filename);
        return 0; 
    }
    
    return original_remove(filename);
}