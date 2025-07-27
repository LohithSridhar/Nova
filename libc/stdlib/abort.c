#include <stdio.h>
#include <stdlib.h>
#if defined(__is_libk)
#include <kernel/tty.h>
//#include "vga.h" // Still too VGA centric, although the colours are being changed
#endif

__attribute__((__noreturn__))
void abort(char* reason) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	screen_setcolor((uint8_t) 4); // 4, 0
	printf("NovaOS 0.0.5: kernel: PANIC: abort()\n");
	screen_setcolor((uint8_t) 12); // 12, 0
	printf("Reason: %s\n", reason);
	screen_setcolor((uint8_t) 192); // 0, 12
	printf("FATAL ERROR\n");
	screen_setcolor((uint8_t) 4); // 4, 0 return fg_color | bg_color << 4;
	printf("Restart the machine to reset system.\n", reason);
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort() due to Reason:%s\n", reason);
#endif
	while (1) { }
	__builtin_unreachable();
}

