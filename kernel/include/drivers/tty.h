#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    TTY_COLOR_BLACK = 0,
    TTY_COLOR_BLUE,
    TTY_COLOR_GREEN,
    TTY_COLOR_CYAN,
    TTY_COLOR_RED,
    TTY_COLOR_MAGENTA,
    TTY_COLOR_YELLOW,
    TTY_COLOR_WHITE,
    TTY_COLOR_GRAY,       // Optional: light gray
    TTY_COLOR_BRIGHT_BLUE,
    TTY_COLOR_BRIGHT_GREEN,
    TTY_COLOR_BRIGHT_CYAN,
    TTY_COLOR_BRIGHT_RED,
    TTY_COLOR_BRIGHT_MAGENTA,
    TTY_COLOR_BRIGHT_YELLOW,
    TTY_COLOR_BRIGHT_WHITE,
    TTY_COLOR_DEFAULT = 15 // Used as a default/fallback
} tty_color_t;

/* entry_color
 * PURPOSE - take foreground and background colour inputs and unabstract to VGA standard
 * INPUT - tty_color_t fg_color (text colour), tty_color_t bg_color (highlight color)
 * OUPTUT - uint8_t color (colour entry that can be put into screenbuffer) */
uint8_t entry_color(tty_color_t, tty_color_t);

/* vga_entry
 * PURPOSE - take colour from vga_entry_color and character and convert into printable entry
 * INPUT - uchar character (character to be printed), uint8_t color (text and highlight colour)
 * OUPTUT - uint16_t entry (2 byte entry ready to be put into screenbuffer) */
uint16_t vga_entry(unsigned char, uint8_t);

/* tty_initialize
 * PURPOSE - blank out anything that GRUB or BIOS put onscreen and reset text colour
 * INPUT - NULL
 * OUPTUT - NULL (screenbuffer is blanked out) */
void tty_initialize(void);

/* tty_putchar
 * PURPOSE - print character at cursor and move cursor
 * INPUT - char c (any ASCII/UTF-8 character)
 * OUPTUT - NULL (character is printed and cursor is moved) */
void tty_putchar(char);

/* tty_setcolor
 * PURPOSE - change text colour to inputted colour
 * INPUT - uint8_t color (exactly what it sounds like) 
 * OUPTUT - NULL (text colour is changed)
 * TODO: make this non-sensitive to VGA */
void tty_setcolor(uint8_t);

/* tty_writestring
 * PURPOSE - take inputted string and write to screenbuffer
 * INPUT - const char* data (string to be written)
 * OUPTUT - NULL (string is written to screenbuffer) */
void tty_writestring(const char*);

#endif