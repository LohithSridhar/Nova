#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#if defined(__is_libk)
#include <drivers/tty.h>
#endif
typedef bool (*putchar_func_t)(char ch, void* ctx);

#if defined(__is_libk)
bool ANSI_parse(const char* ANSI_escape_sequence) {
	size_t escseq_length = strlen(ANSI_escape_sequence);
	int current_argument = 0;
	switch (ANSI_escape_sequence[escseq_length - 1]) {
		case 'm':
			while (*ANSI_escape_sequence != 'm') {
				current_argument = 0;
				while (*ANSI_escape_sequence != ';' && *ANSI_escape_sequence != 'm') {
					current_argument = current_argument * 10 + (*ANSI_escape_sequence - '0');
					ANSI_escape_sequence++;
				}
				ANSI_escape_sequence++;
				if (current_argument >= 108 || current_argument < 0) break;
				switch (current_argument) {
					case 0: tty_setcolor(entry_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK)); break;
					case 1: tty_setcolor(tty_getcolor() + 8); break;
					case 30: tty_setcolor(entry_color(TTY_COLOR_BLACK, tty_getcolor() >> 4)); break;
					case 31: tty_setcolor(entry_color(TTY_COLOR_RED, tty_getcolor() >> 4)); break;
					case 32: tty_setcolor(entry_color(TTY_COLOR_GREEN, tty_getcolor() >> 4)); break;
					case 33: tty_setcolor(entry_color(TTY_COLOR_YELLOW, tty_getcolor() >> 4)); break;
					case 34: tty_setcolor(entry_color(TTY_COLOR_BLUE, tty_getcolor() >> 4)); break;
					case 35: tty_setcolor(entry_color(TTY_COLOR_MAGENTA, tty_getcolor() >> 4)); break;
					case 36: tty_setcolor(entry_color(TTY_COLOR_CYAN, tty_getcolor() >> 4)); break;
					case 37: tty_setcolor(entry_color(TTY_COLOR_WHITE, tty_getcolor() >> 4)); break;
					case 40: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BLACK)); break;
					case 41: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_RED)); break;
					case 42: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_GREEN)); break;
					case 43: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_YELLOW)); break;
					case 44: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BLUE)); break;
					case 45: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_MAGENTA)); break;
					case 46: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_CYAN)); break;
					case 47: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_WHITE)); break;
					case 90: tty_setcolor(entry_color(TTY_COLOR_GRAY, tty_getcolor() >> 4)); break;
					case 91: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_RED, tty_getcolor() >> 4)); break;
					case 92: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_GREEN, tty_getcolor() >> 4)); break;
					case 93: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_YELLOW, tty_getcolor() >> 4)); break;
					case 94: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_BLUE, tty_getcolor() >> 4)); break;
					case 95: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_MAGENTA, tty_getcolor() >> 4)); break;
					case 96: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_CYAN, tty_getcolor() >> 4)); break;
					case 97: tty_setcolor(entry_color(TTY_COLOR_BRIGHT_WHITE, tty_getcolor() >> 4)); break;
					case 100: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_GRAY)); break;
					case 101: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BRIGHT_RED)); break;
					case 102: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BRIGHT_GREEN)); break;
					case 103: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BRIGHT_YELLOW)); break;
					case 104: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BRIGHT_BLUE)); break;
					case 105: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BRIGHT_MAGENTA)); break;
					case 106: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BRIGHT_CYAN)); break;
					case 107: tty_setcolor(entry_color(tty_getcolor() & 0xF, TTY_COLOR_BRIGHT_WHITE)); break;
					default: break; // Unsupported
				}
			}
			break;
		default:
			return false;
	}
	return true;
}
#else
static bool ANSI_parse(const char *ANSI_escape_sequence) { return false; }
#endif

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

	int position = 0;
	char pad_char = zero_pad ? '0' : ' ';

	for (int i = 0; i < pad_len; i++)
		buffer[position++] = pad_char;
	for (int i = idx; i < 8; i++)
		buffer[position++] = temp[i];
	buffer[position] = '\0';

	return position;
}

typedef struct {
	putchar_func_t putch;
	void *ctx;
	bool ansi_enabled;
	int written; // count chars written
} print_context_t;

static bool putc_context(char ch, print_context_t* pc) {
	if (!pc->putch(ch, pc->ctx)) {
		return false; // output failure
	}
	pc->written++;
	return true;
}

static int vkprintf(print_context_t* pc, const char* format, va_list args) {
	pc->written = 0;
	while (*format != '\0') {
		if (pc->ansi_enabled && format[0] == '\x1B' && format[1] == '[') {
			// Parse ANSI escape sequence
			format += 2;
			char ANSI_esc_sequence[32] = "";
			int i = 0;
			while (*format && (isdigit(*format) || *format == ';') && i < 31) {
				ANSI_esc_sequence[i++] = *format++;
			}
			if (*format) {
				ANSI_esc_sequence[i++] = *format++;
			}
			ANSI_esc_sequence[i] = '\0';

			if (!ANSI_parse(ANSI_esc_sequence)) {
				// If not handled, print raw escape sequence
				putc_context('\x1B', pc);
				putc_context('[', pc);
				for (int j = 0; j < i; j++) {
					putc_context(ANSI_esc_sequence[j], pc);
				}
			}

			continue;
		}

		if (*format != '%' || (format[1] == '%')) {
			if (*format == '%')
				format++; // skip first % for %%
			if (!putc_context(*format++, pc))
				return -1;
			continue;
		}

		format++; // skip '%'
		bool zero_pad = false;
		int width = 0;

		if (*format == '0') {
			zero_pad = true;
			format++;
		}

		while (isdigit(*format)) {
			width = width * 10 + (*format - '0');
			format++;
		}

		// Handle specifiers
		switch (*format) {
			case 'z':
				if (*(format + 1) == 'u') {
					format += 2;
					size_t u = va_arg(args, size_t);
					char str[21];
					int i = 0;
					if (u == 0)
						str[i++] = '0';
					else {
						size_t n = u;
						int num_len = 0;
						size_t tmp = n;
						while (tmp) {
							tmp /= 10;
							num_len++;
						}
						str[i + num_len] = '\0';
						while (n) {
							str[i + num_len - 1] = '0' + (n % 10);
							n /= 10;
							num_len--;
						}
					}
					for (int j = 0; str[j] != '\0'; j++) {
						if (!putc_context(str[j], pc)) return -1;
					}
				} else goto unknown_specifier;
				break;

			case 'c': {
				format++;
				char c = (char)va_arg(args, int);
				if (!putc_context(c, pc))
					return -1;
				break;
			}

			case 's': {
				format++;
				const char* str = va_arg(args, const char*);
				size_t len = strlen(str);
				for (size_t i = 0; i < len; i++) {
					if (!putc_context(str[i], pc)) return -1;
				}
				break;
			}

			case 'd': {
				format++;
				int num = va_arg(args, int);
				char str[15];
				int i = 0;
				if (num == 0) {
					str[i++] = '0';
					str[i] = '\0';
				} else {
					int n = num;
					if (n < 0) {
						str[i++] = '-';
						n = -n;
					}
					int temp = n, num_len = 0;
					while (temp) {
						temp /= 10;
						num_len++;
					}
					str[i + num_len] = '\0';
					while (n) {
						str[i + num_len - 1] = (n % 10) + '0';
						n /= 10;
						num_len--;
					}
				}
				for (int j = 0; str[j] != '\0'; j++) {
					if (!putc_context(str[j], pc)) return -1;
				}
				break;
			}

			case 'x': case 'X': {
				char caps = *format;
				format++;
				unsigned int uint_for_hex = va_arg(args, unsigned int);
				char str[11];
				int len = uint_to_hex(str, sizeof(str), uint_for_hex, width, zero_pad, caps == 'X');
				if (len == EOF) return -1;
				for (int i = 0; i < len; i++) {
					if (!putc_context(str[i], pc)) return -1;
				}
				break;
			}

			case 'p': {
				format++;
				void* pointer = va_arg(args, void*);
				uintptr_t address = (uintptr_t)pointer;
				char str[2 + sizeof(uintptr_t)*2 + 1];
				int position = 0;
				str[position++] = '0';
				str[position++] = 'x';
				bool started = false;
				for (int i = (int)(sizeof(uintptr_t)*2) - 1; i >= 0; i--) {
					int shift = i * 4;
					char digit = "0123456789ABCDEF"[(address >> shift) & 0xF];
					if (digit != '0' || started || i == 0) {
						started = true;
						str[position++] = digit;
					}
				}
				str[position] = '\0';
				for (int i = 0; i < position; i++) {
					if (!putc_context(str[i], pc)) return -1;
				}
				break;
			}

			default:
			unknown_specifier:
				// print unknown sequence literally
				if (!putc_context('%', pc)) return -1;
				if (!putc_context(*format, pc)) return -1;
				format++;
				break;
		}
	}

	return pc->written;
}

#if defined(__is_libk)
static bool tty_printf_putchar(char ch, void* ctx) {
	(void)ctx;
	tty_putchar(ch);
	return true;
}

static bool tty_printf_putchar_fini(void) {
	tty_update_cursor_location();
	return true;
}
#else
static bool tty_printf_putchar(char ch, void* ctx) {
	(void)ctx;
	// TODO: user space putchar
	return true;
}

static bool tty_printf_putchar_fini(void) {
	// TODO: user space putchar_fini
	return true;
}
#endif

struct strbuf {
	char* buffer;
	size_t position;
	size_t size;
};

static bool strbuf_putchar(char ch, void* ctx) {
	struct strbuf* sb = (struct strbuf*)ctx;
	if (sb->position < sb->size - 1) { // reserve space for NUL
		sb->buffer[sb->position++] = ch;
		return true;
	}
	return false; // no space left
}

int vprintf(const char* format, va_list args) {
	print_context_t pc = { .putch = tty_printf_putchar, .ctx = NULL, .ansi_enabled = true, .written = 0 };
	int return_value = vkprintf(&pc, format, args);
	tty_printf_putchar_fini();
	return return_value;
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int ret = vprintf(format, args);
	va_end(args);
	return ret;
}

int vsprintf(char* buffer, const char* format, va_list args) {
	struct strbuf sb = { .buffer = buffer, .position = 0, .size = SIZE_MAX };
	print_context_t pc = { .putch = strbuf_putchar, .ctx = &sb, .ansi_enabled = false, .written = 0 };
	int ret = vkprintf(&pc, format, args);
	sb.buffer[sb.position] = '\0';
	return ret;
}

int sprintf(char* buffer, const char* format, ...) {
	va_list args;
	va_start(args, format);
	int ret = vsprintf(buffer, format, args);
	va_end(args);
	return ret;
}

// printf.c - Why do there have to be so many xprintxs??? The way this works is that vkprintf is a
// catch-all that checks for format specifiers, and has functions like vsprintf and vprintf derive from it.