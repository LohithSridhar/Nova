#include <stdio.h>
#include <stdlib.h>
#if defined(__is_libk)
#include <kernel/tty.h>
#include "vga.h" // Still too VGA centric
#endif

__attribute__((__noreturn__))
void abort(char* reason) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	screen_setcolor(vga_entry_color(VGA_RED, VGA_BLACK));
	printf("NovaOS 0.0.5: kernel: PANIC: abort()\n");
	screen_setcolor(vga_entry_color(VGA_LIGHTRED, VGA_BLACK));
	printf("Reason: %s\n", reason);
	screen_setcolor(vga_entry_color(VGA_BLACK, VGA_LIGHTRED));
	printf("FATAL ERROR\n");
	screen_setcolor(vga_entry_color(VGA_RED, VGA_BLACK));
	printf("Restart the machine to reset system.\n", reason);
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort() due to Reason:%s\n", reason);
#endif
	while (1) { }
	__builtin_unreachable();
}

