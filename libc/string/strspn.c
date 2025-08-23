#include <string.h>
#include <stdbool.h>
#include <stdio.h>

int strspn(const char *astr, const char *bstr) {
	size_t astr_len = strlen(astr) - 1;
	size_t bstr_len = strlen(bstr);
	bool found = false;
	for (int i = 0; i < astr_len; i++) {
		found = true;
		for (int j = 0; j < bstr_len; j++) {
			if (astr[i] == bstr[j]) found = false;
		}
		if (found) return i;
	}

	return 0;
}

// strspn.c - strspn is somehow harder to make than strcspan, but it's just a reverse, looking for the first instance that
// DOESN'T have the keychars found in bstr, instead of looking for the ones that DO.