#include <string.h>

char *strcpy(char *dest, const char *src) {
	char *ret = dest;          // save the start of dest to return
	while ((*dest++ = *src++) != '\0') {}
	return ret;
}

// strcpy.c - strcpy does the exact same thing as memcpy, but working on a str, giving it a speed and memory boost.