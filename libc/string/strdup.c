#include <stdlib.h>
#include <string.h>

char *strdup(const char *str) {
    size_t len = strlen(str);
    char *new_str = malloc(len + 1);
    if (!new_str) return NULL;
    strcpy(new_str, str);
    return new_str;
}

// strdup.c - This function duplicates a string and returns the duplicate, which is stored in the heap by malloc.