#pragma once
#include <stdbool.h>

/* tty_change_leftright_position
 * PURPOSE - Change the left and right position of the cursor
 * INPUT - int amount (If negative, to the left, if positive, to the right)
 * OUTPUT - NULL (cursor position is changed) */
void tty_change_leftright_position(int);

/* tty_backspace_squish
 * PURPOSE - Squish the excess lines to the right of a backspace to the left
 * INPUT - int amount (Amount of characters to squish by)
 * OUTPUT - NULL (text is squished) */
void tty_backspace_squish(int);

/* tty_scroll_down
 * PURPOSE - Scroll the window into the screen_buffer down
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down by one) */
void tty_scroll_down(void);

/* tty_scroll_up
 * PURPOSE - Scroll the window into the screen_buffer up
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls up by one) */
void tty_scroll_up(void);

/* tty_scroll_start
 * PURPOSE - Scroll the window into the screen_buffer up to the start
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way up) */
void tty_scroll_start(void);

/* tty_scroll_end
 * PURPOSE - Checks if you're at the end.
 * INPUT - NULL
 * OUTPUT - bool at_end (self-explanatory) */
bool tty_at_end(void);

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
void tty_scroll_end(void);

// cursor.h - The dual-driver file working between the keyboard and tty to control the screen's cursor using the arrow keys.