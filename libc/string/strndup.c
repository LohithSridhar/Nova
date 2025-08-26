#include <stdlib.h>
#include <string.h>

char *strndup(const char *str, size_t len) {
    char *new_str = malloc(len + 1);
    if (!new_str) return NULL;
    strncpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

// strndup.c - strndup, like all the strn functions, just strdup but with a size limit on it.