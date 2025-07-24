#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
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