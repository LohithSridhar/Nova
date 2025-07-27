#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>
#include "vga.h"

/* tty.c - screen interface (VGA screen buffer) defines all the low-level functions'
 * mechanisms that allow them to interact with the screen. Arch-based Driver. */

// VGA Screen Buffer Dimensions and Memory Location
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Cursor Settings
static size_t screen_row;
static size_t screen_column;
static uint8_t screen_color;
static uint16_t* screen_buffer; // Actual screen buffer

void screen_initialize(void) { // FIXME: WHY IS THE BLINKER (CURSOR MARKER) SOMEWHERE ELSE???
	screen_row = 0;
	screen_column = 0;
	screen_color = vga_entry_color(VGA_LIGHTGRAY, VGA_BLACK);
	screen_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			screen_buffer[index] = vga_entry(' ', screen_color);
		}
	}
}

void screen_setcolor(uint8_t color) {
	screen_color = color;
}

void screen_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	screen_buffer[index] = vga_entry(c, color);
}

void screen_putchar(char c) { // Different from putentryat in that you don't have to
	unsigned char uc = c;     // specify anything about colour or location
	if (uc == '\n') {
		screen_column = 0;
		if (++screen_row == VGA_HEIGHT)
			screen_row = 0;
		return;
	}
	screen_putentryat(uc, screen_color, screen_column, screen_row);
	if (++screen_column == VGA_WIDTH) {
		screen_column = 0;
		if (++screen_row == VGA_HEIGHT)
			screen_row = 0;
	}

	return;
}

void screen_write(const char* data, size_t size) { // Internal, only used by writestring
	for (size_t i = 0; i < size; i++)
		screen_putchar(data[i]);
}

void screen_writestring(const char* data) {
	screen_write(data, strlen(data));
}