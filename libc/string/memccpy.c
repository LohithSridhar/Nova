#include <string.h>

void *memccpy(void* restrict dstptr, const void* restrict srcptr, int character, size_t size) {
	void *pointer = memchr(srcptr, character, size);
    if (pointer) {
        return (void *) ((char*) memcpy(dstptr, srcptr, pointer - srcptr + 1) + (pointer - srcptr) + 1);
    }

    memcpy(dstptr, srcptr, size);
    return NULL;
}

// memccpy.c - memccpy is like memcpy, but copies up to the first instance of a character. It has the same stompover problems as memcpy.