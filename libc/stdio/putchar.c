#include <stdio.h>
#if defined(__is_libk)
#include <drivers/tty.h>
#endif
int putchar(int ic) {
#if defined(__is_libk)
	char c[2] = { (char) ic, '\0' };
	tty_writestring(c);
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}

// putchar.c - This function takes in a character and prints it to the screen. This will be replaced by a system call in libc.