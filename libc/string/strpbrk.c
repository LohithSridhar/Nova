#include <string.h>

char *strpbrk(const char *astr, const char *bstr) {
	size_t astr_len = strlen(astr) - 1;
	size_t bstr_len = strlen(bstr) - 1;
	for (int i = 0; i < astr_len; i++) {
		for (int j = 0; j < bstr_len; j++) {
			if (astr[i] == bstr[j]) return (char *) (astr + i);
		}
	}

	return NULL;
}

// strpbrk.c - strpbrk is basically just strcspn, but instead of returning the index, it skips that and returns a pointer.