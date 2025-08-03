#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
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

		if (format[0] != '%' || format[1] == '%') { // This is regular printing (no %)
			if (format[0] == '%')                   // You only come here if the string has "%%" in it
				format++;                           // This skips the first % and prints the other one
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') // This checks for the amount of text until
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
		while (*format >= '0' && *format <= '9') {
		    width = width*10 + (*format - '0');
		    format++;
		}

		if (*format == 'c') {                       // %c for character (aka char or uchar)
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
			void* pointer = va_arg(parameters, void*);
			uintptr_t uint_for_hex = (uintptr_t) pointer;
			char str[13];
			str[0] = '0';
			str[1] = 'x';
			size_t len = (size_t) uint_to_hex(&str[2], sizeof(str) - 2, (unsigned int) uint_for_hex, 10, true, true);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW
				return -1;
			}
			if (!print(str, len))
				return -1;
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