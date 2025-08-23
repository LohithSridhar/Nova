#include <string.h>

int strcmp(const char* astr, const char* bstr) {
	while (*astr && (*astr == *bstr)) {astr++; bstr++;}
	return (unsigned char) *astr - (unsigned char) *bstr;
}

// strcmp.c - strcmp does the exact same thing as memcmp, but working on a str, giving it a speed and memory boost.