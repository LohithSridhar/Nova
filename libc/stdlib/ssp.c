#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/osname.h>

uintptr_t __stack_chk_guard_var = 0;
uintptr_t *__stack_chk_guard __attribute__((weak, visibility("default"))) = &__stack_chk_guard_var;

__attribute__((__noreturn__))
void __stack_chk_fail(void) {
	abort("Stack Smashing Detected", OS_NAME);
	while (1) {}
	__builtin_unreachable();
}

#if defined(__is_libk)
void __ssp_init(uint32_t seed) {
	if (__stack_chk_guard_var == 0) __stack_chk_guard_var = 0xDEADBEEF;
	else __stack_chk_guard_var = seed;
	return;
}
#else
void __ssp_init(uint32_t seed) {
	return;
}
#endif