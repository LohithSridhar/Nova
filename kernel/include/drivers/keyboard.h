#pragma once
#include <stddef.h>
#include <stdbool.h>

#define NOP (char) 0x13 // This is the char for a read scancode where you do NOTHING (but something still happened)

/* entry_color
 * PURPOSE - Clears the keyboard input buffer and readies it for future input
 * INPUT - NULL
 * OUTPUT - NULL */
void clear_buffer(void);

/* entry_color
 * PURPOSE - Initialises the keyboard by clearing the buffer and also resetting some flags
 * INPUT - NULL
 * OUTPUT - NULL */
void keyboard_initialize(void);

/* entry_color
 * PURPOSE - I don't know WHY, but the driver thinks the first debug scancode is a NOP. Therefore, this exists
 * INPUT - NULL
 * OUTPUT - NULL */
void keyboard_debug_init(void);

/* entry_color
 * PURPOSE - Read the oldest character in the buffer. If there is none (or there's an error) it returns a -1.
 * INPUT - NULL
 * OUTPUT - char character_read (the oldest character in the buffer) */
char read_buffer(void);

/* read_keyboard
 * PURPOSE - Read a message buflen long from the keyboard to the buffer. Options for multiline and silent typing.
 * INPUT - char *buffer (buffer to read to), size_t buflen (amount to read), bool silent (replaces the letters
 * with a simple white block (IBM CP437 number 219)), and bool multiline (makes [ENTER] add \n, and [SHIFT]+[ENTER] exit.)
 * OUTPUT - nothing, as the message is READ into buffer instead. */
void read_keyboard(char*, size_t, bool, bool);

// keyboard.h - the API for the libk and kernel to interfac with the keyboard. Primarily uses read_keyboard.