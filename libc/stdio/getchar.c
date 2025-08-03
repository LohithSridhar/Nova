#include <stdio.h>
#if defined(__is_libk)
#include <drivers/keyboard.h>
#endif

int getchar() {
#if defined(__is_libk)
	char c[2];
	read_keyboard(c, 2, false, false);
	if (c[0] != '\0') return (int)c[0];
#else
	// TODO: implement system calls.
#endif
	return EOF;
}