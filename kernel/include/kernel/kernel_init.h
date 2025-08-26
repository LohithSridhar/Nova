#pragma once

#include <stdint.h>
#include <stdbool.h>

/* kernel_init
 * PURPOSE - Initialises many basic kernel features
 * INPUT - NULL
 * OUTPUT - NULL */
void kernel_init(void);

/* random_init
 * PURPOSE - Initialise (or, in some cases, reinitialise) the RNG
 * INPUT - NULL
 * OUTPUT - NULL */
void random_init(void);

/* halt
 * PURPOSE - Mainly for debug; halts the computer using cli / hlt
 * INPUT - NULL
 * OUTPUT - NULL */
__attribute__((__noreturn__))
void halt(void);

/* breakpoint
 * PURPOSE - Halts all execution and starts Neutrino
 * INPUT - NULL
 * OUTPUT - NULL */
void breakpoint(void);

/* set_in_breakpoint_true
 * PURPOSE - Self-explanatory (Neutrino auto-sets this to false when exiting)
 * INPUT - NULL
 * OUTPUT - NULL */
void set_in_breakpoint_true(void);

/* in_a_breakpoint
 * PURPOSE - Checks if in a breakpoint
 * INPUT - NULL
 * OUTPUT - bool in_a_breakpoint */
bool in_a_breakpoint(void);

/* currently_in_a_breakpoint
 * PURPOSE - For some ungodly reason, we need two of these for different features (one for IKD init and one for the IKD)
 * INPUT - NULL
 * OUTPUT - bool currently_in_a_breakpoint */
bool currently_in_a_breakpoint(void);

// kernel_init.h - A bit random in places, isn't it? That's because this handles a lot of general kernel features, mainly
// things that will eventually go somewhere else, like breakpoints, the timer, and starting up and shutting down.