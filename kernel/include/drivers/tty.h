#pragma once
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
    TTY_COLOR_WHITE, // Other name: Light Grey
    TTY_COLOR_GRAY, // Other name: DARK Grey???
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
 * PURPOSE - Take foreground and background colour inputs and unabstract to VGA standard
 * INPUT - tty_color_t fg_color (text colour), tty_color_t bg_color (highlight color)
 * OUTPUT - uint8_t color (colour entry that can be put into screenbuffer) */
uint8_t entry_color(tty_color_t, tty_color_t);

/* tty_entry
 * PURPOSE - Take colour from entry_color and character and convert into printable entry
 * INPUT - uchar character (character to be printed), uint8_t color (text and highlight colour)
 * OUTPUT - uint16_t entry (2 byte entry ready to be put into screenbuffer) */
uint16_t tty_entry(unsigned char, uint8_t);

/* tty_initialize
 * PURPOSE - Blank out anything that GRUB or BIOS put onscreen and reset text colour
 * INPUT - NULL
 * OUTPUT - NULL (screenbuffer is blanked out) */
void tty_initialize(void);

/* tty_putchar
 * PURPOSE - Print character at cursor and move cursor
 * INPUT - char c (any ASCII/UTF-8 character)
 * OUTPUT - NULL (character is printed and cursor is moved) 
 * NOTES - Doesn't update the cursor; for that, use writestring or manually do so. */
void tty_putchar(char);

/* tty_update_cursor_location
 * PURPOSE - Self-explanatory. Note that you can't set, only update.
 * INPUT - NULL
 * OUTPUT - NULL */
void tty_update_cursor_location(void);

/* tty_setcolor
 * PURPOSE - Change text colour to inputted colour
 * INPUT - uint8_t color (exactly what it sounds like) 
 * OUTPUT - NULL (text colour is changed) */
void tty_setcolor(uint8_t);

/* tty_getcolor
 * PURPOSE - Retrieve current text colour
 * INPUT - NULL
 * OUTPUT - uint8_t text_color (exactly what it sounds like) */
uint8_t tty_getcolor(void);

/* tty_writestring
 * PURPOSE - Take inputted string and write to screenbuffer
 * INPUT - const char* data (string to be written)
 * OUTPUT - NULL (string is written to screenbuffer) */
void tty_writestring(const char*);

/* tty_save_screen
 * PURPOSE - Save the state of the screen_buffer (even the unseen stuff) and any extra flags
 * INPUT - NULL
 * OUTPUT - NULL (all the data is saved to the secondary copy) */
void tty_save_screen(void);

/* tty_load_screen
 * PURPOSE - Load the data and flags from the copy to the screen_buffer (yes, the unseen stuff)
 * INPUT - NULL
 * OUTPUT - NULL (all the data is loaded from the secondary copy) */
void tty_load_screen(void);

// tty.h - The primary API between libk/kernel and the screen, in the form of a DOS-LIKE text UI in the screen (see the TTY UI)