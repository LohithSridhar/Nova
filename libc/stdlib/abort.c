#include <stdio.h>
#include <stdlib.h>
#if defined(__is_libk)
extern void halt(void);
#endif

__attribute__((__noreturn__))
void abort(const char* cause, const char *process_name) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	printf("\x1B[31;40m%s: kernel: PANIC: abort()\n", process_name);
	printf("\x1B[1mCause: %s\n", cause);
	printf("\x1B[30;101m%s\n", "FATAL ERROR");
	printf("\x1B[0;31m%s\n", "Restart the machine to reset the system.");
	halt();
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("%s: abort() due to cause:%s\n", process_name, cause);
#endif
	while (1) {};
	__builtin_unreachable();
}

// abort.c - also called panic, as it's what causes the kernel panic, abort is the function that terminates a program. This is
// still to be done, as we don't have program-to-program interfaces and commands like SIGABRT and SIGKILL yet.