#include "io.h"
#include <stdint.h>

inline void outb(uint16_t port, uint8_t value) {
	__asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile ("inb %1, %0"
					  : "=a"(ret)
					  : "Nd"(port));
	return ret;
}

// io.c - The (yet again, arch-specific) C file for communicating with ports (outb is outputting bytes, inb is inputting bytes).