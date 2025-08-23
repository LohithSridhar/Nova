#include <string.h>

char *strchr(const char *str, int value) {
	int i = 0;
	for (; str[i] && str[i] != value; i++) {}
	return (char *) (str + i);
}

// strchr.c - strchr does the exact same thing as memchr, but working on a str, giving it a speed and memory boost.