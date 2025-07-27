.globl gdt_flush
.globl stack_top_address

# Define the stack top address as a constant - see below
# .equ stack_top_address, 0x200000

.text

gdt_flush:
	# Standard cdecl prologue
	push %ebp
	mov %esp, %ebp

	# Load pointer to gdt_ptr (argument at [ebp+8])
	mov 8(%ebp), %eax

	# Load GDT using lgdt [eax]
	lgdt (%eax)

flush2:
	# Reload data segment registers with selector 0x10
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	# Move esp to stack_top_address - We will probably do this seperately
	# mov $stack_top_address, %esp

	# Epilogue and return
	pop %ebp
	ret