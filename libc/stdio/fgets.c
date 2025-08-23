#include <stdio.h>
#include <stddef.h>
#include <string.h>
#if defined(__is_libk)
#include <drivers/keyboard.h>
#endif

char* fgets(char *buffer, size_t size) { // TODO: STREAM/FILE variable for userspace
#if defined(__is_libk)
	read_keyboard(buffer, size-1, false, false);
	memset(buffer + size-2, '\n', 1);
	memset(buffer + size-1, '\0', 1);
	return buffer;
#else
	// TODO: system call
#endif
	return "";
}

// fgets.c - This is the function that reads an string from the keyboard. It is one of many to require a system call in libc.