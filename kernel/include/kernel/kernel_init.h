#pragma once

#include <stdint.h>
#include <stdbool.h>

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
void kernel_init(void);

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
int get_rand(void);

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
void halt(void);

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
void breakpoint(void);

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
void set_in_breakpoint_true(void);

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
bool in_a_breakpoint(void);

/* tty_scroll_end
 * PURPOSE - Scroll the window into the screen_buffer down to the end
 * INPUT - NULL
 * OUTPUT - NULL (screen_buffer window scrolls down all the way down) */
bool currently_in_a_breakpoint(void);

// kernel_init.h - A bit random in places, isn't it? That's because this handles a lot of general kernel features, mainly
// things that will eventually go somewhere else, like breakpoints, the timer, and starting up and shutting down.