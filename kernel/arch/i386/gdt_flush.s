.globl gdt_flush
.section .text

gdt_flush:
	mov 4(%esp), %eax    # pointer to gdt_ptr
	lgdt (%eax)

	# Long jump to reload CS and flush pipeline
	ljmp $0x08, $flush2

flush2:
	mov $0x10, %ax       # 0x10 = GDT entry 2 = data segment
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	ret

# gdt_flush - The file responsible for flushing the GDT and reinitializing it in the CPU when creating the OS' GDT.
