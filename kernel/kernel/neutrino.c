#include <kernel/neutrino.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/osname.h>

#define IS_SPECIAL (c == 0x00 || c == 0xFF || c == 0x0A || c == 0x08)
#define IS_PRINTABLE (c >= 0x20 && c <= 0x7E)
#define IS_EXT_PRINTABLE (c >= 0x80 && c <= 0xFE)

extern void print_heap_header_data(void);
extern void print_heap_memory_data(void);

void print_memory(void *addr, size_t len) {
	uint8_t *p = (uint8_t *)addr;
	putchar(' '); putchar(218); for (int i = 3; i <= 61; i++) putchar(196);
	putchar(210); for (int i = 63; i < 79; i++) putchar(196); putchar(191);
    putchar('\n');
	for (size_t i = 0; i < len; i += 16) {
		putchar(' ');
		putchar(179);
		printf(" %08X: ", (unsigned)(uintptr_t)(p + i));
		for (size_t j = 0; j < 16; j++)
			if (i + j < len-1) printf("%02X ", p[i+j]);
			else if (i + j < len) printf("%02X", p[len-1]);
			else printf(" XX"); // padding
		if ((i + 16) >= len) putchar(' ');
		putchar(186);
		for (size_t j = 0; j < 16; j++) {
			uint8_t c = p[i+j];
			if (i + j < len) printf("%c", !IS_SPECIAL && (IS_PRINTABLE || IS_EXT_PRINTABLE) ? c : '.');
			else printf(" ");
		}

		putchar(179);
		putchar('\n');
	}

	putchar(' '); putchar(192); for (int i = 3; i <= 61; i++) putchar(196);
	putchar(208); for (int i = 63; i < 79; i++) putchar(196); putchar(217);
	putchar('\n');
}

int convert_to_hex(int value) {
	// This assumes that the value is a hex number with all digits in decimal (eg. 0x16 -> 0x10)
	int result = 0;
	int multiplier = 1;

	while (value > 0) {
		int digit = value % 16;
		if (digit > 9) {
			// Invalid
			return 0xFFFFFFFF;
		}

		result += digit * multiplier;
		multiplier *= 10;
		value /= 16;
	}

	return result;
}

void debugger() {
	bool exit = false;
	char command[15];
	char arg1[12];
	unsigned int arg2 = 0;
	unsigned int arg3 = 1;
	while (!exit) {
		printf("\x1B[32;40mroot> \x1B[0m");
		scanf("%s %s %X %X", command, arg1, &arg2, &arg3);
		for (int i = 0; command[i] != '\0'; i++) command[i] = tolower(command[i]);
		for (int i = 0; arg1[i] != '\0'; i++) arg1[i] = tolower(arg1[i]);
		printf("\x1B[94;40m");
		if (!strcmp(command, "exit") || !strcmp(command, "quit") || !strcmp(command, "continue") || !strcmp(command, "c")) {
			exit = true;
			break;
		} else if ((!strcmp(command, "print") || !strcmp(command, "p"))&& *arg1) {
			if (!strcmp(arg1, "heap_headers") || !strcmp(arg1, "heapheaders") || !strcmp(arg1, "heap-headers") ||
			!strcmp(arg1, "hh")) {printf("printing heap header data...\n"); print_heap_header_data();}
			if (!strcmp(arg1, "heap_memory") || !strcmp(arg1, "heapmemory") || !strcmp(arg1, "heap-memory") ||
			!strcmp(arg1, "hm")) {printf("printing heap memory data...\n"); print_heap_memory_data();}
			else if (!strcmp(arg1, "registers") || !strcmp(arg1, "regs") || !strcmp(arg1, "reg") ||
			!strcmp(arg1, "r")) {printf("printing registers...\n"); print_registers();}
			else if (!strcmp(arg1, "memory") || !strcmp(arg1, "mem") || !strcmp(arg1, "m")) {printf("printing memory...\n");
			printf("0d%p 0x%X\n", (void *) arg2, (size_t) arg3); print_memory((void *) arg2, (size_t) arg3);}
			else if (!strcmp(arg1, "stack") || !strcmp(arg1, "s")) {printf("printing stack...\n"); print_stack();}
		} else if (!strcmp(command, "convert") || !strcmp(command, "conv") || !strcmp(command, "cnv")) {
			if (!strcmp(arg1, "hexadecimal") || !strcmp(arg1, "hex") || !strcmp(arg1, "h"))
				printf("hexadecimal(%X) = 0x%X\n", arg2, convert_to_hex(arg2));
			if (!strcmp(arg1, "decimal") || !strcmp(arg1, "dec") || !strcmp(arg1, "d"))
				printf("decimal(0x%X) = %d\n", arg2, arg2);
		} else if (!strcmp(command, "abort") || !strcmp(command, "panic")) {
			abort("Neutrino caused panic.", OS_NAME);
		} else if (!strcmp(command, "version") || !strcmp(command, "ver") || !strcmp(command, "v")) {
			printf("%s version %s\n", OS_NAME, OS_VERSION);
		} else if (!strcmp(command, "help") || !strcmp(command, "man") || !strcmp(command, "h")) {
			printf("Neutrino - IKD for NovaOS - Manual\n");
			printf("Available commands:\nexit/quit/continue/c - quit Neutrino and continues the program.\n");
			printf("help/man/h - print out this instruction manual.\nabort/panic - cause a kernel panic.\n");
			printf("version/ver/v - print the name and version number of the OS.\n");
			printf("convert/conv/cnv - convert between base-16 and base-10 based on next argument:\n");
			printf("\x1B[34;40m\thexadecimal/hex/h - convert a base-10 number to base-16.\n");
			printf("\tdecimal/dec/d - convert a base-16 number to base-10.\n");
			printf("\x1B[94;40mprint/p - print relevant data. Available print subcommands:\n");
			printf("\x1B[34;40m\tstack/s - print stack region.\n\tregisters/regs/reg/r - print registers.\n");
			printf("\tmemory - print memory region based on next arguments:\n\t\tmemory region start\n\t\tmemory region size\n");
			printf("\theap_headers/hh - print the header information of all heap blocks.\n\theap_memory/hm - ");
			printf("print the header and memory info of the blocks.\n");
			printf("\x1B[94;40mNote that any underscores (_) are interchangable with dashes (-) or nothing ().\n");
		} else {
			printf("\x1B[35;40mInvalid command: ");
			printf("\x1B[91;40m%s\n", command);
			printf("\x1B[35;40mType \"help\" for valid commands.\n");
		}
	}
	return;
}

extern void neutrino_init(void);
extern void neutrino_fini(void);

void enter_neutrino() {
	neutrino_init();
	printf("\x1B[90;40m%s - Neutrino, the Internal Kernel Debugger\n", OS_NAME);

	debugger();

	neutrino_fini();
	return;
}

// neutrino.c - The architecture-agnostic part of Neutrino. Contains the Debugging shell and the heap and memory printfs.