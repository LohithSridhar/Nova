#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <drivers/tty.h>
#include "../../io.h"

/* tty.c - screen interface (VGA screen buffer) defines all the low-level functions'
 * mechanisms that allow them to interact with the screen. Arch-based Driver. */

// VGA Screen Buffer Dimensions and Memory Location

// vga_color - VGA colour standard definition. Very cursed.
typedef enum {
	VGA_BLACK = 0,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,            // Damn, I wish I could call this teal
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHTGRAY,
	VGA_DARKGRAY,
	VGA_LIGHTBLUE,       // I call it sky but light works as well
	VGA_LIGHTGREEN,      // Too saturated to be "light" but ok
	VGA_LIGHTCYAN,       // Isn't this Cyan?
	VGA_LIGHTRED,
	VGA_LIGHTMAGENTA,
	VGA_LIGHTBROWN,      // Look, I get that this is a standard but
	VGA_WHITE = 15       // WHAT THE HELL???
} vga_color;

vga_color tty_to_vga_color(tty_color_t col) {
    // Maps abstract TTY colors to VGA's color space.
    static const uint8_t table[] = {
        VGA_BLACK,      // TTY_COLOR_BLACK
        VGA_RED,        // TTY_COLOR_RED
        VGA_GREEN,      // TTY_COLOR_GREEN
        VGA_BROWN,      // TTY_COLOR_YELLOW
        VGA_BLUE,       // TTY_COLOR_BLUE
        VGA_MAGENTA,    // TTY_COLOR_MAGENTA
        VGA_CYAN,       // TTY_COLOR_CYAN
        VGA_LIGHTGRAY,  // TTY_COLOR_WHITE
        VGA_DARKGRAY,   // TTY_COLOR_GRAY
        VGA_LIGHTRED,   // TTY_COLOR_BRIGHT_RED
        VGA_LIGHTGREEN, // TTY_COLOR_BRIGHT_GREEN
        VGA_LIGHTBROWN,// TTY_COLOR_BRIGHT_YELLOW
        VGA_LIGHTBLUE,  // TTY_COLOR_BRIGHT_BLUE
        VGA_LIGHTMAGENTA, // TTY_COLOR_BRIGHT_MAGENTA
        VGA_LIGHTCYAN,  // TTY_COLOR_BRIGHT_CYAN
        VGA_WHITE       // TTY_COLOR_BRIGHT_WHITE, TTY_COLOR_DEFAULT
    };
    if (col < sizeof(table)/sizeof(*table))
        return table[col];
    return VGA_WHITE; // fallback
}

inline uint8_t entry_color(tty_color_t fg_color, tty_color_t bg_color) {
	return fg_color | bg_color << 4;
}

inline uint16_t vga_entry(unsigned char character, uint8_t color) {
	return (uint16_t) character | (uint16_t) color << 8;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Cursor Settings
static size_t tty_row;
static size_t tty_column;
static uint8_t tty_color;
static uint16_t* tty_buffer; // Actual screen buffer

void set_cursor_location(int x, int y) {
	uint16_t position = y*VGA_WIDTH+x;

	outb(VGA_CTRL_REGISTER, 0x0F);
	outb(VGA_DATA_REGISTER, (uint8_t)(position & 0xFF));

	outb(VGA_CTRL_REGISTER, 0x0E);
	outb(VGA_DATA_REGISTER, (uint8_t)((position >> 8) & 0xFF));

	return;
}

void tty_initialize(void) {
	tty_row = 0;
	tty_column = 0;
	set_cursor_location((int) tty_column, (int) tty_row);
	tty_color = entry_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);
	tty_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			tty_buffer[index] = vga_entry(' ', tty_color);
		}
	}
}

void tty_setcolor(uint8_t color) {
	tty_color = color;
}

void tty_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	tty_buffer[index] = vga_entry(c, color);
}

void tty_putchar(char c) { // Different from putentryat in that you don't have to
	unsigned char uc = c;     // specify anything about colour or location
	if (uc == '\n') {
		tty_column = 0;
		if (++tty_row == VGA_HEIGHT)
			tty_row = 0;
		set_cursor_location(tty_column < VGA_WIDTH ? ((int) tty_column) : 0,
		tty_row + 1 < VGA_HEIGHT ? (tty_column < VGA_WIDTH ? (int) tty_row : (
		(int) tty_row) + 1) : (tty_column < VGA_WIDTH ? (int) tty_row : 0)); // what.
		return;
	} else if (uc == '\b') {
		if (tty_column == 0) {
			tty_column = VGA_WIDTH-1;
			if (tty_row == 0) {
				tty_row = VGA_HEIGHT-1;
			} else {
				tty_row--;
			}
		} else {
			tty_column--;
		}
		tty_putentryat(' ', tty_color, tty_column, tty_row);
		set_cursor_location(tty_column, tty_row); // what (again).
		return;
	}
	tty_putentryat(uc, tty_color, tty_column, tty_row);
	if (++tty_column == VGA_WIDTH) {
		tty_column = 0;
		if (++tty_row == VGA_HEIGHT)
			tty_row = 0;
	}

	if (uc == 219) return;

	set_cursor_location(tty_column < VGA_WIDTH ? ((int) tty_column) : 0,
	tty_row + 1 < VGA_HEIGHT ? (tty_column < VGA_WIDTH ? (int) tty_row : (
	(int) tty_row) + 1) : (tty_column < VGA_WIDTH ? (int) tty_row : 0)); // what (yet again).

	return;
}

void tty_write(const char* data, size_t size) { // Internal, only used by writestring
	for (size_t i = 0; i < size; i++)
		tty_putchar(data[i]);
}

void tty_writestring(const char* data) {
	tty_write(data, strlen(data));
}