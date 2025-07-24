#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/tty.h>
#include "gdt.h"

void early_kernel_main(void);

void kernel_main(void) {
    early_kernel_main();
    char *string = "Hello!"; // strlen(string) = 6
    screen_initialize();
    printf("Welcome to NovaOS Version 0.0.5!\n\n");
    screen_writestring("Currently, we are completing unit tests.\nputchar - ");
    int zero = putchar(48); // should print '0'
    putchar(zero + 1); // should print '1'
    puts("\nputs - Does this work?");
    puts("puts - Is this on a seperate line?");
    printf("printf - The char putchar printed is %c (in ASCII %d), and an example string is:\n%s   %%\n", zero, 48, string);
    char a = 0b00011111;
    char b = 0b00100000;
    printf("memcmp - memcmp(31, 32) = %d, memcmp(32, 31) = %d, memcmp(32, 32) = %d\n", memcmp(&a, &b, sizeof(char)),
                                                                                       memcmp(&b, &a, sizeof(char)),
                                                                                       memcmp(&b, &b, sizeof(char)));
    char c = 0b10101010;
    printf("memcpy and memmove - c = %d. ", c);
    memmove(&c, &a, sizeof(char));
    printf("memmove(c, 31) -> c = %d. ", c);
    memcpy(&c, &b, sizeof(char));
    printf("memcpy(c, 32) -> c = %d.", c);
    printf("memset - c before memset = %d, ", c);
    memset(&c, 4, sizeof(char));
    printf("c after memset(4) = %d.\n", c);
    printf("strlen - the length of the string '%s' is %d\n", string, strlen(string));
    abort("Final Unit Test - abort/panic. Unit tests are complete. NovaOS 0.0.5,\nturning off.");
    return;
}

void early_kernel_main(void) {
    screen_initialize();
    gdt_init();
    screen_initialize();
    /* TODO: idt_init and then __asm__ ("sti");*/
    return;
}