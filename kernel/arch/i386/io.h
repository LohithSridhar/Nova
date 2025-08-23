#pragma once
#include <stdint.h>

void outb(uint16_t, uint8_t); // Write outwards to a port (the first is the port to write to, the second is the data)
uint8_t inb(uint16_t); // Read inwards from a port (The only arg is the port to read, and the return is the data.)

// io.h - io.c's respective header file, made only for 32-bit-elf-x86 files with specific setups.