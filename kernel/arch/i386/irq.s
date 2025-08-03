.section .text
.globl irq0_handler
.globl irq1_handler
.globl irq_default_handler
.extern irq_handler

# IRQ0 (timer)
irq0_handler:
	pusha
	push $0           # Push IRQ number
	call irq_handler
	add $4, %esp
	popa

	movb $0x20, %al   # Send EOI to master PIC
	outb %al, $0x20
	iret

# IRQ1 (keyboard)
irq1_handler:
	pusha
	push $1
	call irq_handler
	add $4, %esp
	popa

	movb $0x20, %al
	outb %al, $0x20
	iret

# Fallback for all other IRQs/exceptions
irq_default_handler:
	cli
.halt_forever:
	hlt
	jmp .halt_forever