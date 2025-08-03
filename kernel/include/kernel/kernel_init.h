#ifndef _KERNEL_INIT_H
#define _KERNEL_INIT_H

#include <stdint.h>

void kernel_init(void);
int get_rand(void);
void halt(void);

#endif
// kernel_init.h - barren, isn't it? It's simple, as you only have to initialize once, and that is handled by one function.
// also, don't ask me why the halt function is in the init script. I have no idea either.