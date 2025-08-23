#include <string.h>
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

// strlen.c - strlen is possibly the simplest string function, simply getting the length of a string until the terminator NULL.