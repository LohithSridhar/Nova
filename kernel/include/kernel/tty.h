#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

/* screen_initialize
 * PURPOSE - blank out anything that GRUB or BIOS put onscreen and reset text colour
 * INPUT - NULL
 * OUPTUT - NULL (screenbuffer is blanked out) */
void screen_initialize(void);

/* screen_putchar
 * PURPOSE - print character at cursor and move cursor
 * INPUT - char c (any ASCII/UTF-8 character)
 * OUPTUT - NULL (character is printed and cursor is moved) */
void screen_putchar(char c);

/* screen_setcolor
 * PURPOSE - change text colour to inputted colour
 * INPUT - uint8_t color (exactly what it sounds like) 
 * OUPTUT - NULL (text colour is changed)
 * TODO: make this non-sensitive to VGA */
void screen_setcolor(uint8_t color);

/* screen_writestring
 * PURPOSE - take inputted string and write to screenbuffer
 * INPUT - const char* data (string to be written)
 * OUPTUT - NULL (string is written to screenbuffer) */
void screen_writestring(const char* data);

#endif