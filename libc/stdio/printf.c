#include <limits.h>
#include <sys/cdefs.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <kernel/kernel_init.h>
#include <stdio.h>
#include <string.h>
#if defined(__is_libk)
#include <drivers/tty.h>
#endif

static bool print(const char* data, size_t length) {
#if defined(__is_libk)
	char truncated_data[length+1];
	memcpy(truncated_data, data, length+1); // AAAAGH WHY DO I HAVE TO DO THIS
	truncated_data[length] = '\0';
	tty_writestring(truncated_data);
#else
	// TODO: Implement stdio and the write system call.
#endif
	return true;
}

static bool ANSI_parse(const char *ANSI_escape_sequence) {
#if defined(__is_libk)
	// Print has already truncated the escape sequence start and made a clean string for us!
	size_t escseq_length = strlen(ANSI_escape_sequence);
	int current_argument = 0;
	//printf("%s\n", ANSI_escape_sequence);
	switch (ANSI_escape_sequence[escseq_length-1]) {
		case 'm':
			// This is the SGR (Select Graphics Rendition) command
			while (*ANSI_escape_sequence != 'm') {
				current_argument = 0;
				while (*ANSI_escape_sequence != ';' && *ANSI_escape_sequence != 'm') {
					current_argument = current_argument * 10 + (*ANSI_escape_sequence - '0');
					ANSI_escape_sequence++;
				}
				ANSI_escape_sequence++;
				if (current_argument >= 108 || current_argument < 0) break;
				// Now, we have our ANSI argument.

				switch (current_argument) {
					case 0: tty_setcolor(entry_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK)); break;
					// For now, we only need to reset this. Later, we will need to reset more things.
					case 1: tty_setcolor(tty_getcolor()+8); break; // Brighten! break;
					// OH NO OH NO OH NO OH NO WHY DOES MY TTY_COLOR HAVE TO BE DIFFERENT FROM THE ANSI CODES AAAAA
					case 30: tty_setcolor(entry_color(TTY_COLOR_BLACK, tty_getcolor()>>4)); break;
					case 31: tty_setcolor(entry_color(TTY_COLOR_RED, tty_getcolor()>>4)); break;
					case 32: tty_setcolor(entry_color(TTY_COLOR_GREEN, tty_getcolor()>>4)); break;
					case 33: tty_setcolor(entry_color(TTY_COLOR_YELLOW, tty_getcolor()>>4)); break;
					case 34: tty_setcolor(entry_color(TTY_COLOR_BLUE, tty_getcolor()>>4)); break;
					case 35: tty_setcolor(entry_color(TTY_COLOR_MAGENTA, tty_getcolor()>>4)); break;
					case 36: tty_setcolor(entry_color(TTY_COLOR_CYAN, tty_getcolor()>>4)); break;
					case 37: tty_setcolor(entry_color(TTY_COLOR_WHITE, tty_getcolor()>>4)); break;
					case 40: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BLACK)); break;
					case 41: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_RED)); break;
					case 42: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_GREEN)); break;
					case 43: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_YELLOW)); break;
					case 44: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BLUE)); break;
					case 45: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_MAGENTA)); break;
					case 46: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_CYAN)); break;
					case 47: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_WHITE)); break;
					case 90: tty_setcolor(entry_color(TTY_COLOR_GRAY, tty_getcolor()>>4)); break;
					case 91: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_RED, tty_getcolor()>>4)); break;
					case 92: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_GREEN, tty_getcolor()>>4)); break;
					case 93: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_YELLOW, tty_getcolor()>>4)); break;
					case 94: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_BLUE, tty_getcolor()>>4)); break;
					case 95: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_MAGENTA, tty_getcolor()>>4)); break;
					case 96: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_CYAN, tty_getcolor()>>4)); break;
					case 97: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_WHITE, tty_getcolor()>>4)); break;
					case 100: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_GRAY)); break;
					case 101: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BRIGHT_RED)); break;
					case 102: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BRIGHT_GREEN)); break;
					case 103: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BRIGHT_YELLOW)); break;
					case 104: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BRIGHT_BLUE)); break;
					case 105: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BRIGHT_MAGENTA)); break;
					case 106: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BRIGHT_CYAN)); break;
					case 107: tty_setcolor(entry_color(tty_getcolor()&0xF, TTY_COLOR_BRIGHT_WHITE)); break;
					default: break; // Not supported yet :(
				}
			}

			break;
		default:
			return false; // Not supported yet :(
	} // IT'S FINALLY DONE
#else
	// TODO: Yadda Yadda something something system call.
#endif
	return true;
}

static int uint_to_hex(char* buffer, size_t bufsize, unsigned int num, int width, bool zero_pad, bool uppercase) {
    static const char digits_lower[] = "0123456789abcdef";
    static const char digits_upper[] = "0123456789ABCDEF";
    const char* digits = uppercase ? digits_upper : digits_lower;

    char temp[9];
    int idx = 8;
    temp[idx] = '\0';

    if (num == 0) {
        temp[--idx] = '0';
    } else {
        while (num && idx > 0) {
            temp[--idx] = digits[num & 0xF];
            num >>= 4;
        }
    }

    int hex_len = 8 - idx;
    int pad_len = (width > hex_len) ? (width - hex_len) : 0;
    int total_len = pad_len + hex_len;

    if (total_len + 1 > (int)bufsize) // +1 for null terminator
        return EOF; // buffer too small

    int pos = 0;
    // pad character
    char pad_char = zero_pad ? '0' : ' ';

    for (int i = 0; i < pad_len; i++)
        buffer[pos++] = pad_char;

    for (int i = idx; i < 8; i++)
        buffer[pos++] = temp[i];

    buffer[pos] = '\0';

    return pos;
}

int printf(const char* restrict format, ...) { // This logic is really confusing
	va_list parameters;                        // so I'm gonna explain any tricky parts
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] == '\x1B' && format[1] == '[') {
			// This is an ANSI Escape Code.
			format += 2;
			
			char ANSI_esc_sequence[32] = "";
			int i = 0;
			while (*format && (isdigit(*format) || *format == ';') && i < 32) {
				ANSI_esc_sequence[i] = *format;
				i++; format++;
			}
			ANSI_esc_sequence[i] = *format;
			ANSI_esc_sequence[i+1] = '\0';
			format++;

			ANSI_parse(ANSI_esc_sequence);
			continue;
		}

		if (format[0] != '%' || format[1] == '%') { // This is regular printing (no %)
			if (format[0] == '%')                   // You only come here if the string has "%%" in it
				format++;                           // This skips the first % and prints the other one
			size_t amount = 1;
			while (format[amount] && format[amount] != '%' && format[amount] != '\x1B')
															// This checks for the amount of text until
				amount++;                                   // The next % letter aka the printable stuff
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW. (We need to get an IDT or similar)
				return -1;
			}
			if (!print(format, amount))             // I know that this looks like a conditional but it's
				return -1;                          // Printing and checking if it was successful.
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;     // If we have a %, check the next character.

		// After seeing '%'
		bool zero_pad = false;
		int width = 0;

		// Check for '0' flag (zero padding)
		if (*format == '0') {
		    zero_pad = true;
		    format++;
		}

		// Parse numeric width
		while (isdigit(*format)) {
		    width = width*10 + (*format - '0');
		    format++;
		}

		if (*format == 'z' && *(format + 1) == 'u') {
		    format += 2;
		    size_t u = va_arg(parameters, size_t);
		    char str[21]; // Enough for 64-bit size_t
		    int i = 0;
		    if (u == 0) {
		        str[i++] = '0';
		        str[i] = '\0';
		    } else {
		        size_t n = u;
		        int num_len = 0;
		        size_t tmp = n;
		        while (tmp) { tmp /= 10; num_len++; }
		        str[i + num_len] = '\0';
		        while (n) {
		            str[i + num_len - 1] = '0' + (n % 10);
		            n /= 10;
		            num_len--;
		        }
		    }
		    size_t len = strlen(str);
		    if (maxrem < len) return -1;
		    if (!print(str, len)) return -1;
		    written += len;
		} else if (*format == 'c') {                       // %c for character (aka char or uchar)
			format++;
			char c = (char) va_arg(parameters, int);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {                // %s for string (aka char*, char[], uchar*, or uchar[])
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			int num = va_arg(parameters, int);
			char str[12];
			int i = 0;
			if (num == 0) {
				str[i++] = '0';
				str[i] = '\0';
			}

			int n = num;
			if (n < 0) {
				str[i++] = '-';
				n = -n;
			}

			int temp = n, num_len = 0;
			while (temp) { temp /= 10; num_len++; }
			str[i + num_len] = '\0';
			while (n) {
				str[i + num_len - 1] = (n % 10) + '0';
				n /= 10;
				num_len--;
			}

			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'x' || *format == 'X') {
			char caps = *format;
			format++;
			unsigned int uint_for_hex = va_arg(parameters, unsigned int);
			char str[10];
			size_t len = (size_t) uint_to_hex(str, 10, uint_for_hex, width, zero_pad, caps == 'X');
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'p') {
			format++;
			void *pointer = va_arg(parameters, void *);
			uintptr_t addr = (uintptr_t)pointer;

			// Account for 32 or 64 bit
			char str[2 + sizeof(uintptr_t) * 2 + 1]; // "0x" + 16 hex + NUL
			str[0] = '0';
			str[1] = 'x';
			int pos = 2;
			bool started = false;
			for (int i = (sizeof(uintptr_t) * 2) - 1; i >= 0; i--) {
				int shift = i * 4;
				char digit = "0123456789abcdef"[(addr >> shift) & 0xf];
				if (digit != '0' || started || i == 0) {
					started = true;
					str[pos++] = digit;
				}
			}
			str[pos] = '\0';
			size_t len = pos;
			if (maxrem < len) return -1;
			if (!print(str, len)) return -1;
			written += len;
		} else {                                    // TODO: add %f/F, %A/%a, and %e (%E is a crime)
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

// printf.c - This is either the 1st or 2nd most used function in the C programming language. It takes in a format string and
// outputs a clean printed set of data to the screen, with support for numbers, hex, octal, strings, pointers, and characters!