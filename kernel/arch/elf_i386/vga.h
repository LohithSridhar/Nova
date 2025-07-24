#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>

// vga_color - VGA colour standard definition. Very cursed.
enum vga_color {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3, // Damn, I wish I could call this teal
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHTGRAY = 7,
    VGA_DARKGRAY = 8,
    VGA_LIGHTBLUE = 9, // I call it sky but light works as well
    VGA_LIGHTGREEN = 10, // Too saturated to be "light" but ok
    VGA_LIGHTCYAN = 11, // Isn't this Cyan?
    VGA_LIGHTRED = 12,
    VGA_LIGHTMAGENTA = 13,
    VGA_LIGHTBROWN = 14, // Look, I get that this is a standard but
    VGA_WHITE = 15       // WHAT THE HELL???
};

/* vga_entry_color
 * PURPOSE - take foreground and background colour inputs and unabstract to VGA standard
 * INPUT - enum vga_color fg_color (text colour), enum vga_color bg_color (highlight color)
 * OUPTUT - uint8_t color (colour entry that can be put into screenbuffer) */
static inline uint8_t vga_entry_color(enum vga_color fg_color, enum vga_color bg_color) {
	return fg_color | bg_color << 4;
}

/* vga_entry
 * PURPOSE - take colour from vga_entry_color and character and convert into printable entry
 * INPUT - uchar character (character to be printed), uint8_t color (text and highlight colour)
 * OUPTUT - uint16_t entry (2 byte entry ready to be put into screenbuffer) */
static inline uint16_t vga_entry(unsigned char character, uint8_t color) {
	return (uint16_t) character | (uint16_t) color << 8;
}

#endif