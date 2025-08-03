#include <stddef.h>
#include <stdbool.h>

void clear_buffer(void);
void keyboard_initialize(void);
void read_keyboard(char*, size_t, bool, bool);
const char read_buffer(void);