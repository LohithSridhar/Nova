ENTRY(_start)

SECTIONS
{
    /* Start at 1MiB */
    . = 0x100000;

    .text ALIGN(4K) :
    {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata*)
    }

    .data ALIGN(4K) :
    {
        *(.data*)
    }

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
    }

    /* Define heap immediately after .bss and aligned */
    .heap ALIGN(4K) :
    {
        __heap_start = .;
        /* Reserve 960KB for heap (0xF0000) to avoid overlapping stack */
        . = . + 0xF0000;
        __heap_end = .;
    }

    /* Align and place GDT, TSS, IDT after heap */
    . = ALIGN(4K);

    .gdt :
    {
        *(.gdt)
    }

    . = ALIGN(4K);

    .tss :
    {
        *(.tss)
    }

    . = ALIGN(4K);

    .idt :
    {
        *(.idt)
    }

    /* Define stack region explicitly with symbols, just after IDT */
    . = ALIGN(4K);
    PROVIDE(stack_top = . + 0x4000); /* 16 KB stack size */
    PROVIDE(stack_bottom = .);

    /* No actual section content for stack as stack grows down from stack_top */
}

/* link.ld - The linker file designed to link the entire kernel together. Temporary until the initrd and diskutil work. */