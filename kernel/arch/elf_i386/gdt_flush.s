; gdt_flush.asm -- NASM version of GDT loader/segment reload for a 32-bit x86 flat kernel

[BITS 32]

global gdt_flush
extern stack_top_address

section .text

gdt_flush:
	; cdecl: Argument [esp+4] (after call pushes return address)
	push ebp
	mov ebp, esp
	mov eax, [ebp+8] ; fetch pointer to gdt_ptr

	; The pointer to the GDT (struct gdt_ptr) is now in EAX
	lgdt [eax]

flush2:
	; Reload data segment registers with the selector for your data segment.
	mov ax, 0x10 ; 0x10 = offset 2*8, which is typically the data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov esp, stack_top_address

	pop ebp
	ret