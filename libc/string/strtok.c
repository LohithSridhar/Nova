#include <string.h>
#include <stddef.h>

static int is_delim(char c, const char *delim) {
	while (*delim) {
		if (c == *delim) return 1;
		delim++;
	}
	return 0;
}

char *strtok(char *str, const char *delim) {
	static char *next;
	char *start;
	
	if (str) {
		next = str;
	} else if (!next) {
		return NULL;
	}
	
	while (*next && is_delim(*next, delim)) {
		next++;
	}
	
	if (*next == '\0') {
		next = NULL;
		return NULL;
	}
	
	start = next;
	
	while (*next && !is_delim(*next, delim)) {
		next++;
	}
	
	if (*next) {
		*next = '\0';
		next++;
	} else {
		next = NULL;
	}
	
	return start;
}

// strtok.c - strtok doesn't mean it's string tiktok, it means that it tokenises the strings (basically splitting it by a
// "delineator", a special character that it should split the string across. VERY useful in parsing lists and compilers.