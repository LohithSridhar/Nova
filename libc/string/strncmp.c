#include <string.h>

int strncmp(const char *astr, const char *bstr, size_t length) {
	for (int i = 0; i < length && astr[i] && bstr[i]; i++) {
		if (astr[i] < bstr[i]) return -1;
		else if (astr[i] > bstr[i]) return 1;
	}

	return 0;
}

// strncmp.c - strncmp is just strcmp, but with the ability to only cmp n strings instead of all of them.