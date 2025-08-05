#include <stdio.h>
#include <stdlib.h>
#if defined(__is_libk)
#include <drivers/tty.h>
extern void halt(void);
#endif

__attribute__((__noreturn__))
void abort(char* cause) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	tty_setcolor(entry_color(TTY_COLOR_RED, TTY_COLOR_BLACK)); // 4, 0
	printf("NovaOS 0.1.7: kernel: PANIC: abort()\n");
	tty_setcolor(entry_color(TTY_COLOR_BRIGHT_RED, TTY_COLOR_BLACK)); // 12, 0
	printf("Cause: %s\n", cause);
	tty_setcolor(entry_color(TTY_COLOR_BLACK, TTY_COLOR_BRIGHT_RED)); // 0, 12
	printf("FATAL ERROR\n");
	tty_setcolor(entry_color(TTY_COLOR_RED, TTY_COLOR_BLACK)); // 4, 0 return fg_color | bg_color << 4;
	printf("Restart the machine to reset system.\n", cause);
	halt();
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort() due to cause:%s\n", cause);
#endif
	while (1) {};
	__builtin_unreachable();
}

