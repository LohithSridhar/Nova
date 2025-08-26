#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <drivers/tty.h>
#include <kernel/cursor.h>
#include <kernel/kernel_init.h>
#include "../../io.h"

// VGA Screen Buffer Dimensions and Memory Location
// vga_color - VGA colour standard definition. Very cursed.
typedef enum {
	VGA_BLACK = 0,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,            // Damn, I wish I could call this teal
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHTGRAY,
	VGA_DARKGRAY,
	VGA_LIGHTBLUE,       // I call it sky but light works as well
	VGA_LIGHTGREEN,      // Too saturated to be "light" but ok
	VGA_LIGHTCYAN,       // Isn't this Cyan?
	VGA_LIGHTRED,
	VGA_LIGHTMAGENTA,
	VGA_LIGHTBROWN,      // Look, I get that this is a standard but
	VGA_WHITE = 15       // WHAT THE HELL???
} vga_color;

vga_color tty_to_vga_color(tty_color_t col) {
	// Maps abstract TTY colors to VGA's color space.
	static const uint8_t table[] = {
		VGA_BLACK,      // TTY_COLOR_BLACK
		VGA_RED,        // TTY_COLOR_RED
		VGA_GREEN,      // TTY_COLOR_GREEN
		VGA_BROWN,      // TTY_COLOR_YELLOW
		VGA_BLUE,       // TTY_COLOR_BLUE
		VGA_MAGENTA,    // TTY_COLOR_MAGENTA
		VGA_CYAN,       // TTY_COLOR_CYAN
		VGA_LIGHTGRAY,  // TTY_COLOR_WHITE
		VGA_DARKGRAY,   // TTY_COLOR_GRAY
		VGA_LIGHTRED,   // TTY_COLOR_BRIGHT_RED
		VGA_LIGHTGREEN, // TTY_COLOR_BRIGHT_GREEN
		VGA_LIGHTBROWN,// TTY_COLOR_BRIGHT_YELLOW
		VGA_LIGHTBLUE,  // TTY_COLOR_BRIGHT_BLUE
		VGA_LIGHTMAGENTA, // TTY_COLOR_BRIGHT_MAGENTA
		VGA_LIGHTCYAN,  // TTY_COLOR_BRIGHT_CYAN
		VGA_WHITE       // TTY_COLOR_BRIGHT_WHITE, TTY_COLOR_DEFAULT
	};
	if (col < sizeof(table)/sizeof(*table))
		return table[col];
	return VGA_WHITE; // fallback
}

inline uint8_t entry_color(tty_color_t fg_color, tty_color_t bg_color) {
	return fg_color | bg_color << 4;
}

inline uint16_t tty_entry(unsigned char character, uint8_t color) {
	return (uint16_t) character | (uint16_t) color << 8;
}

#define TTY_DEFAULT entry_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK)
#define TTY_DEBUG entry_color(TTY_COLOR_MAGENTA, TTY_COLOR_BRIGHT_MAGENTA) // the inversion is intentional

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define MAX_VGA_INDEX VGA_HEIGHT*VGA_WIDTH-1
#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5
#define SCREEN_STRIDE (VGA_WIDTH * VGA_HEIGHT)

static inline size_t screen_index(size_t x, size_t y) {
	return y * VGA_WIDTH + x;
}

static inline size_t index_to_x(size_t index) {
	return index % VGA_WIDTH;
}

static inline size_t index_to_y(size_t index) {
	return (index - index_to_x(index)) / VGA_WIDTH;
}

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Cursor Settings
static size_t tty_row, tty_column;
static uint16_t cursor_position;
static uint8_t tty_color;
static uint16_t *tty_buffer; // Actual screen buffer

// Our custom screenbuffer
#define SCREENBUFFER_SIZE 500
static uint16_t screen_buffer[SCREENBUFFER_SIZE * SCREEN_STRIDE];
static uint16_t buffer_copy[SCREENBUFFER_SIZE * SCREEN_STRIDE];
static size_t tty_row_copy, tty_column_copy, tty_color_copy;
static uint16_t current_buffer_pos, current_buffer_max, current_buffer_min;
static uint16_t current_buffer_pos_copy, current_buffer_max_copy, current_buffer_min_copy;

static inline void tty_update_buffer(size_t x, size_t y) {
	screen_buffer[current_buffer_pos * VGA_WIDTH + screen_index(x, y)] = tty_buffer[screen_index(x, y)];
}

static void tty_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = screen_index(x, y);
	tty_buffer[index] = tty_entry(c, color);
	tty_update_buffer(x, y);
}

static inline void tty_set_cursor_location(int x, int y) {
	if (screen_index(x, y) <= MAX_VGA_INDEX) cursor_position = screen_index(x, y);
}

void tty_update_cursor_location(void) {
	outb(VGA_CTRL_REGISTER, 0x0F);
	outb(VGA_DATA_REGISTER, (uint8_t)(cursor_position & 0xFF));

	outb(VGA_CTRL_REGISTER, 0x0E);
	outb(VGA_DATA_REGISTER, (uint8_t)((cursor_position >> 8) & 0xFF));

	return;
}

static void tty_clear_screen(void) {
	for (int i = 0; i < SCREENBUFFER_SIZE * VGA_WIDTH; i++) {
		screen_buffer[i] = tty_entry(' ', TTY_DEBUG);
	}

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = screen_index(x, y);
			tty_buffer[index] = tty_entry(' ', TTY_DEFAULT);
			tty_update_buffer(x, y);
		}
	}
}

void tty_initialize(void) {
	tty_row = 0;
	tty_column = 0;
	tty_color = TTY_DEFAULT;
	tty_buffer = VGA_MEMORY;
	current_buffer_pos = 0;
	current_buffer_min = current_buffer_pos;
	current_buffer_max = current_buffer_pos;
	tty_clear_screen();
	tty_set_cursor_location((int) tty_column, (int) tty_row);
	tty_update_cursor_location();
	return;
}

void tty_setcolor(uint8_t color) {
	tty_color = color;
	return;
}

uint8_t tty_getcolor(void) {
	return tty_color;
}

static inline void tty_update_screen(void) {
	memcpy(tty_buffer, screen_buffer + (current_buffer_pos * VGA_WIDTH), SCREEN_STRIDE * sizeof(uint16_t));
}

void tty_save_screen(void) {
	memcpy(buffer_copy, screen_buffer, SCREENBUFFER_SIZE * SCREEN_STRIDE);
	tty_row_copy = tty_row;
	tty_column_copy = tty_column;
	tty_color_copy = tty_color;
	current_buffer_max_copy = current_buffer_max;
	current_buffer_min_copy = current_buffer_min;
	current_buffer_pos_copy = current_buffer_pos;
	return;
}

void tty_load_screen(void) {
	memcpy(screen_buffer, buffer_copy, SCREENBUFFER_SIZE * SCREEN_STRIDE);
	tty_row = tty_row_copy;
	tty_column = tty_column_copy;
	tty_color = tty_color_copy;
	current_buffer_max = current_buffer_max_copy;
	current_buffer_min = current_buffer_min_copy;
	current_buffer_pos = current_buffer_pos_copy;
	tty_update_screen();
	return;
}

void tty_change_leftright_position(int amount) {
	tty_set_cursor_location(tty_column + amount, tty_row);
	tty_column += amount;
	tty_update_cursor_location();
	return;
}

void tty_scroll_up(void) {
	if (current_buffer_pos > current_buffer_min) {
		current_buffer_pos--;
		tty_update_screen();
		tty_set_cursor_location(0, 0);
		tty_update_cursor_location();
	}
	return;
}

void tty_scroll_down(void) {
	if (current_buffer_pos < current_buffer_max) {
		current_buffer_pos++;
		tty_update_screen();
		tty_set_cursor_location(VGA_WIDTH-1, VGA_HEIGHT-1);
		tty_update_cursor_location();
	}
	return;
}

void tty_scroll_start(void) {
	current_buffer_pos = current_buffer_min;
	tty_update_screen();
	tty_set_cursor_location(tty_column, tty_row);
	tty_update_cursor_location();
	return;
}

inline bool tty_at_end(void) {
	return (current_buffer_pos == current_buffer_max);
}

void tty_scroll_end(void) {
	current_buffer_pos = current_buffer_max;
	tty_update_screen();
	tty_set_cursor_location(tty_column, tty_row);
	tty_update_cursor_location();
	return;
}

static void tty_newline(void) {
	tty_column = 0;
	if (++tty_row == VGA_HEIGHT) {
		current_buffer_max++;
		tty_scroll_down();
		for (int i = 0; i < VGA_WIDTH; i++) {
			tty_putentryat(' ', TTY_DEFAULT, i, tty_row-1);
			tty_update_buffer(i, tty_row-1);
		}
		tty_row--;
	}
	tty_set_cursor_location(0, (size_t) tty_row);
	return;
}

static void tty_backspace(void) {
	if (tty_column == 0) {
		tty_column = VGA_WIDTH-1;
		if (tty_row == 0) {
			tty_row = VGA_HEIGHT-1;
		} else {
			tty_row--;
		}
	} else {
		tty_column--;
	}
	tty_set_cursor_location(tty_column, tty_row); // what (again).
	tty_putentryat(' ', tty_color, tty_column, tty_row);
	tty_update_buffer(tty_column, tty_row);
	return;
}

void tty_putchar(char c) {    // Different from putentryat in that you don't have to
	unsigned char uc = c;     // specify anything about colour or location
	if (uc == '\n') {
		tty_newline();
		return;
	} else if (uc == '\b') {
		tty_backspace();
		return;
	} else if (uc == '\t') {
		for (int i = 0; i < 4; i++) tty_putchar(' ');
		return;
	}
	tty_putentryat(uc, tty_color, tty_column, tty_row);
	if (++tty_column == VGA_WIDTH) {
		tty_column = 0;
		if (++tty_row == VGA_HEIGHT) {
			current_buffer_max++;
			tty_scroll_down();
			for (int i = 0; i < VGA_WIDTH; i++) {
				tty_putentryat(' ', TTY_DEFAULT, i, tty_row-1);
				tty_update_buffer(i, tty_row-1);
			}
			tty_row--;
		}
	}

	if (uc == 219) return;

	tty_set_cursor_location((int) tty_column, (int) tty_row); // what (yet again).

	return;
}

void tty_writestring(const char* data) {
	for (size_t i = 0; i < strlen(data); i++)
		tty_putchar(data[i]);
	tty_update_cursor_location();
}

// vga_tty.c - The driver responsible for displaying text mode to the screen in the case of VGA - Video Graphics Array.
// It functions by directly writing to memory region 0xB8000. Has support for scrolling, newlines, arrowkeys, and backspaces.