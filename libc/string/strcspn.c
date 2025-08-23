#include <string.h>

int strcspn(const char *astr, const char *bstr) {
	size_t astr_len = strlen(astr) - 1;
	size_t bstr_len = strlen(bstr) - 1;
	for (int i = 0; i < astr_len; i++) {
		for (int j = 0; j < bstr_len; j++) {
			if (astr[i] == bstr[j]) return i;
		}
	}

	return astr_len;
}

// strcspn.c - strcspn is a function that checks for the first character in astr that is also in bstr, like a keyword finder.