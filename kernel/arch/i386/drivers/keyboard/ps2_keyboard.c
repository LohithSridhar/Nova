#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <drivers/keyboard.h>
#include <kernel/kernel_init.h>
#include <kernel/cursor.h>
#include "../../io.h"

#define KEYBOARD_INPUT_PORT 0x60
#define EO(x) (0xE000 | (x))

typedef enum {
	SC_NONE = 0x00,
	SC_ESC,
	SC_1,
	SC_2,
	SC_3,
	SC_4,
	SC_5,
	SC_6,
	SC_7,
	SC_8,
	SC_9,
	SC_0,
	SC_MINUS,
	SC_EQUALS,
	SC_BACKSPACE,
	SC_TAB,
	SC_Q,
	SC_W,
	SC_E,
	SC_R,
	SC_T,
	SC_Y,
	SC_U,
	SC_I,
	SC_O,
	SC_P,
	SC_LEFT_BRACKET,
	SC_RIGHT_BRACKET,
	SC_ENTER,
	SC_LEFT_CTRL,
	SC_A,
	SC_S,
	SC_D,
	SC_F,
	SC_G,
	SC_H,
	SC_J,
	SC_K,
	SC_L,
	SC_SEMICOLON,
	SC_SINGLE_QUOTE,
	SC_BACK_TICK,
	SC_LEFT_SHIFT,
	SC_BACKSLASH,
	SC_Z,
	SC_X,
	SC_C,
	SC_V,
	SC_B,
	SC_N,
	SC_M,
	SC_COMMA,
	SC_PERIOD,
	SC_FORWARDSLASH,
	SC_RIGHT_SHIFT,
	SC_KEYPAD_MULT,
	SC_LEFT_ALT,
	SC_SPACE,
	SC_CAPS_LOCK,
	SC_F1,
	SC_F2,
	SC_F3,
	SC_F4,
	SC_F5,
	SC_F6,
	SC_F7,
	SC_F8,
	SC_F9,
	SC_F10,
	SC_NUM_LOCK,
	SC_SCROLL_LOCK,
	SC_KEYPAD_7,
	SC_KEYPAD_8,
	SC_KEYPAD_9,
	SC_KEYPAD_MINUS,
	SC_KEYPAD_4,
	SC_KEYPAD_5,
	SC_KEYPAD_6,
	SC_KEYPAD_PLUS,
	SC_KEYPAD_1,
	SC_KEYPAD_2,
	SC_KEYPAD_3,
	SC_KEYPAD_0,
	SC_KEYPAD_PERIOD = 0x53,
	SC_F11 = 0x57,
	SC_F12 = 0x58,
	SC_KEYPAD_ENTER = EO(0x1C),
	SC_RIGHT_CTRL = EO(0x1D),
	SC_RIGHT_ALT = EO(0x38),
	SC_KEYPAD_SLASH = EO(0x35),
	SC_NAV_HOME = EO(0x47),
	SC_ARROW_UP = EO(0x48),
	SC_NAV_PAGEUP = EO(0x49),
	SC_ARROW_LEFT = EO(0x4B),
	SC_ARROW_RIGHT = EO(0x4D),
	SC_NAV_END = EO(0x4F),
	SC_ARROW_DOWN = EO(0x60),
	SC_NAV_PAGEDOWN = EO(0x51),
	SC_NAV_INSERT = EO(0x52),
	SC_NAV_DELETE = EO(0x53),
	SC_LEFT_GUI = EO(0x5B),
	SC_RIGHT_GUI = EO(0x5C),
	SC_MENU_APPS = EO(0x5D),
	SC_POWER = EO(0x5E),
	SC_SLEEP = EO(0x5F),
	SC_WAKE = EO(0x63)
} scan_code_t;
static const char lowercase_scancode_chars[] = {
	[SC_1] = '1',
	[SC_2] = '2',
	[SC_3] = '3',
	[SC_4] = '4',
	[SC_5] = '5',
	[SC_6] = '6',
	[SC_7] = '7',
	[SC_8] = '8',
	[SC_9] = '9',
	[SC_0] = '0',
	[SC_MINUS] = '-',
	[SC_EQUALS] = '=',
	[SC_Q] = 'q',
	[SC_W] = 'w',
	[SC_E] = 'e',
	[SC_R] = 'r',
	[SC_T] = 't',
	[SC_Y] = 'y',
	[SC_U] = 'u',
	[SC_I] = 'i',
	[SC_O] = 'o',
	[SC_P] = 'p',
	[SC_LEFT_BRACKET] = '[',
	[SC_RIGHT_BRACKET] = ']',
	[SC_A] = 'a',
	[SC_S] = 's',
	[SC_D] = 'd',
	[SC_F] = 'f',
	[SC_G] = 'g',
	[SC_H] = 'h',
	[SC_J] = 'j',
	[SC_K] = 'k',
	[SC_L] = 'l',
	[SC_SEMICOLON] = ';',
	[SC_SINGLE_QUOTE] = '\'',
	[SC_BACK_TICK] = '`',
	[SC_BACKSLASH] = '\\',
	[SC_Z] = 'z',
	[SC_X] = 'x',
	[SC_C] = 'c',
	[SC_V] = 'v',
	[SC_B] = 'b',
	[SC_N] = 'n',
	[SC_M] = 'm',
	[SC_COMMA] = ',',
	[SC_PERIOD] = '.',
	[SC_FORWARDSLASH] = '/',
	[SC_SPACE] = ' ',
	[SC_KEYPAD_MULT] = '*',
	[SC_KEYPAD_MINUS] = '-',
	[SC_KEYPAD_PLUS] = '+',
	[SC_KEYPAD_7] = '7',
	[SC_KEYPAD_8] = '8',
	[SC_KEYPAD_9] = '9',
	[SC_KEYPAD_4] = '4',
	[SC_KEYPAD_5] = '5',
	[SC_KEYPAD_6] = '6',
	[SC_KEYPAD_1] = '1',
	[SC_KEYPAD_2] = '2',
	[SC_KEYPAD_3] = '3',
	[SC_KEYPAD_0] = '0',
	[SC_KEYPAD_PERIOD] = '.'
};
static const char uppercase_scancode_chars[] = {
	[SC_1] = '!',
	[SC_2] = '@',
	[SC_3] = '#',
	[SC_4] = '$',
	[SC_5] = '%',
	[SC_6] = '^',
	[SC_7] = '&',
	[SC_8] = '*',
	[SC_9] = '(',
	[SC_0] = ')',
	[SC_MINUS] = '_',
	[SC_EQUALS] = '+',
	[SC_Q] = 'Q',
	[SC_W] = 'W',
	[SC_E] = 'E',
	[SC_R] = 'R',
	[SC_T] = 'T',
	[SC_Y] = 'Y',
	[SC_U] = 'U',
	[SC_I] = 'I',
	[SC_O] = 'O',
	[SC_P] = 'P',
	[SC_LEFT_BRACKET] = '{',
	[SC_RIGHT_BRACKET] = '}',
	[SC_A] = 'A',
	[SC_S] = 'S',
	[SC_D] = 'D',
	[SC_F] = 'F',
	[SC_G] = 'G',
	[SC_H] = 'H',
	[SC_J] = 'J',
	[SC_K] = 'K',
	[SC_L] = 'L',
	[SC_SEMICOLON] = ':',
	[SC_SINGLE_QUOTE] = '"',
	[SC_BACK_TICK] = '~',
	[SC_BACKSLASH] = '|',
	[SC_Z] = 'Z',
	[SC_X] = 'X',
	[SC_C] = 'C',
	[SC_V] = 'V',
	[SC_B] = 'B',
	[SC_N] = 'N',
	[SC_M] = 'M',
	[SC_COMMA] = '<',
	[SC_PERIOD] = '>',
	[SC_FORWARDSLASH] = '?',
	[SC_SPACE] = ' ',
	[SC_KEYPAD_MULT] = '*',
	[SC_KEYPAD_MINUS] = '-',
	[SC_KEYPAD_PLUS] = '+',
	[SC_KEYPAD_7] = '7',
	[SC_KEYPAD_8] = '8',
	[SC_KEYPAD_9] = '9',
	[SC_KEYPAD_4] = '4',
	[SC_KEYPAD_5] = '5',
	[SC_KEYPAD_6] = '6',
	[SC_KEYPAD_1] = '1',
	[SC_KEYPAD_2] = '2',
	[SC_KEYPAD_3] = '3',
	[SC_KEYPAD_0] = '0',
	[SC_KEYPAD_PERIOD] = '.'
};
static const char *functional_scancode_strings[] = {
	[SC_NONE] = "NONEPRSS",
	[SC_ESC] = "ESCAPKEY",
	[SC_BACKSPACE] = "BACKSPCE",
	[SC_TAB] = "TABULATR",
	[SC_ENTER] = "ENTERKEY",
	[SC_LEFT_CTRL] = "LEFTCTRL",
	[SC_LEFT_SHIFT] = "LEFTSHFT",
	[SC_RIGHT_SHIFT] = "RGHTSHFT",
	[SC_LEFT_ALT] = "LEFTALTR",
	[SC_CAPS_LOCK] = "CAPSLOCK",
	[SC_F1] = "FNCTION1",
	[SC_F2] = "FNCTION2",
	[SC_F3] = "FNCTION3",
	[SC_F4] = "FNCTION4",
	[SC_F5] = "FNCTION5",
	[SC_F6] = "FNCTION6",
	[SC_F7] = "FNCTION7",
	[SC_F8] = "FNCTION8",
	[SC_F9] = "FNCTION9",
	[SC_F10] = "FNCTON10",
	[SC_NUM_LOCK] = "NUMLOCK" ,
	[SC_SCROLL_LOCK] = "SCRLLOCK",
	[SC_F11] = "FNCTON11",
	[SC_F12] = "FNCTON12"
};
static const char *extended_scancode_strings[] = {
	[0x1C] = "PADENTER",
	[0x1D] = "RGHTCTRL",
	[0x35] = "KPDSLASH",
	[0x38] = "RGTALTGR",
	[0x47] = "NAVGHOME",
	[0x48] = "ARROWUPK",
	[0x49] = "NAVPAGUP",
	[0x4B] = "ARWRGHTK",
	[0x4D] = "ARRWLFTK",
	[0x4F] = "NAVIGEND",
	[0x50] = "ARRWDWNK",
	[0x51] = "NAVPGDWN",
	[0x52] = "NAVINSRT",
	[0x53] = "NAVDELET",
	[0x5B] = "LEFTLOGO",
	[0x5C] = "RGHTLOGO",
	[0x5D] = "MENUAPPS",
	[0x5E] = "POWERKEY",
	[0x5F] = "SLEEPKEY",
	[0x63] = "WAKEPRES",
	// Add more extended entries as needed
};

static char nop_string[2] = {NOP, '\0'};
static scan_code_t scanbuffer[200];
static uint8_t scanindex; // hold up, is this a semaphore?
static unsigned char debug_trigger;
static bool shift_pressed;
static bool shift_delay; // for some reason, unless we do this terribleness, shift isn't working.
static bool capslock;
static bool caps_delay;
static bool arrowkey_pressed;
static bool overtype;
static scan_code_t key_pressed;

void clear_buffer(void) {
	memset(scanbuffer, 0, sizeof(scanbuffer));
	scanindex = 0;
	return;
}

void keyboard_initialize(void) {
	clear_buffer();
	overtype = false;
	shift_pressed = false;
	debug_trigger = 0;
	scanindex = 0;
	return;
}

void keyboard_debug_init(void) {
	clear_buffer();
	debug_trigger = 0;
	scanindex = 0;
	arrowkey_pressed = false;
}

static const char* get_key_representation(scan_code_t sc_raw, bool extended) {
	scan_code_t sc = sc_raw & 0b10000000 ? sc_raw ^ 0b10000000 : sc_raw;
	if (extended) {
		uint8_t code = sc & 0xFF;
		const char *name = extended_scancode_strings[code];
		if (memcmp(name, "ARROWUPK", 8) == 0) {
			tty_scroll_up();
			return nop_string;
		} else if (memcmp(name, "ARRWDWNK", 8) == 0) {
			tty_scroll_down();
			return nop_string;
		} else if (memcmp(name, "NAVPAGUP", 8) == 0) {
			tty_scroll_start();
			return nop_string;
		} else if (memcmp(name, "NAVPGDWN", 8) == 0) {
			tty_scroll_end();
			return nop_string;
		} else {
			//printf(name ? name : "UNKNOWN_EXTENDED");
			return name ? name : "UNKNOWN_EXTENDED";
		}
	}

	// First try printable chars based on shift_pressed state
	uint8_t index = (uint8_t) (sc & 0xFF); // mask to prevent OoB errors
	char c = '\0';
	if (isalpha((uint8_t) lowercase_scancode_chars[index]) && capslock) {
		c = uppercase_scancode_chars[index];
	} else {
		c = shift_pressed ? uppercase_scancode_chars[index] : lowercase_scancode_chars[index];
	}

	if (index == SC_SPACE) {
		c = '\r'; // This is horrifying. Why I need to do this, I do not know.
	}

	if (c != 0) {
		// Return single-character string for printable char
		static char str[2] = {0, 0};
		str[0] = c;
		return str;
	}

	if (index == SC_LEFT_SHIFT || index == SC_RIGHT_SHIFT || index == SC_CAPS_LOCK) {
		return NULL;
	} else if (index == SC_ENTER) {
		return shift_pressed ? "\0\0" : "\n";
	} else if (index == SC_BACKSPACE) {
		return "\b";
	} else if (index == SC_TAB) {
		return "\t";
	}

	// Otherwise return functional string if any
	const char *fname = functional_scancode_strings[index];
	//printf(fname ? fname : "UNKNOWN");
	if (fname) return fname;

	return "UNKNOWN";
}

char read_buffer(void) {
	if (scanindex == 0 || scanbuffer[0] & 0b10000000) {
		return (char) -1;
	}

	const char* key = get_key_representation(scanbuffer[0], (scanbuffer[0] & 0xE000) == 0xE000);
	if (key[0] == '\0') {
		return '\0';
	}
	if (key == NULL) {
		return (char) -1;
	}
	for (int i = 0; i < 199; i++) scanbuffer[i] = scanbuffer[i+1];
	scanbuffer[199] = 0;
	scanindex--;
	if (debug_trigger >= 3) {
		debug_trigger = 0;
		arrowkey_pressed = false;
	}
	if (strlen(key) == 1) {
		if (!arrowkey_pressed) {
			arrowkey_pressed = key[0] == NOP;
			return (char) key[0];
		} else {
			arrowkey_pressed = false;
			return (char) -1;
		}
	} else {
		if (!arrowkey_pressed) {
			bool left_key = strcmp(key, "ARRWLFTK");
			arrowkey_pressed = (left_key) || (strcmp(key, "ARWRGHTK")); // TODO: merge this with the up/down keys
			return arrowkey_pressed ? (left_key ? 'l' : 'r') : -1;
		} else {
			return (char) -1;
		}
	}
}

void read_keyboard(char *buffer, size_t buflen, bool silent) {
	clear_buffer();
	memset(buffer, '\0', buflen);
	uint16_t length = 0;
	size_t cursor_position_in_field = 0;
	if (silent) putchar(219);
	while (length < buflen) {
		int key = read_buffer();
		if (key == -1 || key == 0xA5 || key == -224 || ((unsigned int) key & 0x000000FF) == 0x0000020 || key == NOP) {
			// Nothing to read. WHY IS THERE AN 0xA5 and -224? Only god and the keyboard know.
			continue;
		} else if (key == '\r') {
			key = ' ';
		} else if (arrowkey_pressed) {
			if (key == 'l') {
				if (cursor_position_in_field > 0) {
					cursor_position_in_field--;
					if (!silent) tty_change_leftright_position(-1);
				}
				continue;
			} else if (key == 'r') {
				if (cursor_position_in_field < length) {
					cursor_position_in_field++;
					if (!silent) tty_change_leftright_position(1);
				}
				continue;
			}
		}
		if (!silent && key != '\b') {
			if (!tty_at_end()) tty_scroll_end();
			if (overtype) putchar(key);
			else {
				if (key == '\0' || key == '\n') putchar('\n');
				else putchar(key);
			}
		}
		if (key == '\0' || key == '\n') {
			length++;
			break; // enter - terminate processing
		} else if (key == '\b') {
			for (int i = cursor_position_in_field; i < length - 1; i++) {
				buffer[i] = buffer[i + 1];
			}

			if (!(cursor_position_in_field > 0 && length > 0)) continue;
			cursor_position_in_field--;
			if (!silent) tty_change_leftright_position(-1);
			
			// Decrease length and null-terminate
			length--;
			buffer[length] = '\0';

			// Reprint the portion of buffer from cursor to end
			if (silent) continue;
			for (size_t i = cursor_position_in_field; i < length; i++) {
				putchar(buffer[i]);
			}

			// Print space to clear leftover character at end
			putchar(' ');

			// Move cursor back to new cursor position
			int move_left_amount = (int)(length - cursor_position_in_field + 1);
			tty_change_leftright_position(-move_left_amount);
			continue;
		}

		// If not enough space for key + null terminator, break
		if ((size_t) (length + 1) >= buflen) {
			break;
		}

		// Append key to buffer
		for (size_t i = length; i > cursor_position_in_field; i--) buffer[i] = buffer[i - 1];
		length++;
		buffer[cursor_position_in_field] = (char) key;
		if (length != cursor_position_in_field+1) {
			for (size_t i = cursor_position_in_field+1; i < length; i++) putchar(buffer[i]);
			if (!silent) tty_change_leftright_position(((int) cursor_position_in_field+1) - ((int) length));
		}
		cursor_position_in_field++;
	}

	// Enter-null-terminate the buffer
	buffer[length+1] = '\0';
	tty_scroll_end();
	
	if (silent) putchar('\b');
	if (length >= buflen) {
		putchar('\n');
	}

	return;
}

void keyboard_interrupt(void) {
	uint16_t scan_code = (uint16_t) inb(KEYBOARD_INPUT_PORT);
	if (shift_delay != shift_pressed) shift_pressed = shift_delay;
	if (caps_delay != capslock) capslock = caps_delay;
	if (scan_code == 0xE0) {
		// Extended Key
		scan_code <<= 8;
		scan_code |= inb(KEYBOARD_INPUT_PORT);
	}

	if (scan_code & 0b10000000) {
		// Key released. For now, we do nothing.
		scan_code ^= 0b10000000;
		if ((scan_code_t) scan_code == SC_RIGHT_SHIFT || (scan_code_t) scan_code == SC_LEFT_SHIFT) {
			shift_delay = false;
			return;
		} else if ((scan_code_t) scan_code == SC_LEFT_CTRL || (scan_code_t) scan_code == SC_LEFT_ALT
			  || (scan_code_t) scan_code == SC_ESC) {
			if (debug_trigger > 0) debug_trigger--;
		}
		key_pressed = SC_NONE;
		return;
	}

	if ((scan_code_t) scan_code == SC_RIGHT_SHIFT || (scan_code_t) scan_code == SC_LEFT_SHIFT) {
		shift_delay = true;
		return;
	} else if ((scan_code_t) scan_code == SC_CAPS_LOCK) {
		caps_delay = !caps_delay;
		return;
	} else if ((scan_code_t) scan_code == SC_LEFT_CTRL || (scan_code_t) scan_code == SC_LEFT_ALT
			  || (scan_code_t) scan_code == SC_ESC) {
		if (debug_trigger < 3) debug_trigger++;
		if (debug_trigger >= 3 && !currently_in_a_breakpoint()) {
			breakpoint();
			debug_trigger = 0;
		}
	}

	if (scanindex < 200) {
		scanbuffer[scanindex] = (scan_code_t) scan_code;
		scanindex++;
	} else {
		for (int i = 0; i < 199; i++) scanbuffer[i] = scanbuffer[i+1];
		scanbuffer[199] = (scan_code_t) scan_code;
	}
	key_pressed = scanbuffer[scanindex-1];
	return;
}

// ps2_keyboard.c - This is the (Extremely Big) Driver for handling keyboard input from the IBM Personal/System 2.
// It works by taking in scan codes using interrupts and processing them into readable ASCII (well, IBM CP 437) characters.