#include <string.h>
#include <stdbool.h>

char *strstr(const char *supstr, const char *substr) {
	size_t substr_length = strlen(substr);
	bool substr_found = false;

	for (int i = 0; supstr[i + substr_length]; i++) {
		if (supstr[i] == substr[0]) {
			for (int j = 1; substr[j] && !substr_found; j++) {
				substr_found = substr[j] == supstr[i+j];
			}

			if (substr_found) return (char *) (supstr + i);
		}
	}

	return NULL;
}

// strstr.c - strstr has the cutest name of them all, and looks for strings within strings, specifically the first instance.