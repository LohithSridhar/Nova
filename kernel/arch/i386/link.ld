ENTRY(_start)

SECTIONS
{
	. = 0x100000;          /* Kernel load address: 1MB */

	.text ALIGN(4K) : {
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) : { *(.rodata*) }
	.data   ALIGN(4K) : { *(.data*) }
	.bss    ALIGN(4K) : { *(COMMON) *(.bss) }

	/* Explicitly align start of GDT, TSS, and IDT */
	. = ALIGN(0x1000);
	.gdt : { *(.gdt) }

	. = ALIGN(0x1000);
	.tss : { *(.tss) }

	. = ALIGN(0x1000);
	.idt : { *(.idt) }

	PROVIDE(stack_bottom = 0x200000 - 16*1024);
	PROVIDE(stack_top = 0x200000);
}