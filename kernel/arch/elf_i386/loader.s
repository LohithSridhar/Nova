[BITS 16]

extern early_kernel_main
global loader

KERNEL_STACK_SIZE equ 4096

MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0
CHECKSUM equ -(MAGIC_NUMBER + FLAGS)

section .bss
align 16
kernel_stack: resb KERNEL_STACK_SIZE

section .data
global stack_top_address
stack_top_address:
    dd kernel_stack + KERNEL_STACK_SIZE

section .gdt
align 8
gdt_start:
    dq 0x0000000000000000             ; Null descriptor
    dq 0x00CF9A000000FFFF             ; Code segment descriptor: base=0, limit=4GB, exec/read, ring 0
    dq 0x00CF92000000FFFF             ; Data segment descriptor: base=0, limit=4GB, read/write, ring 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1        ; Limit (size of GDT - 1)
    dd gdt_start                      ; Base address of GDT

section .realmode
align 16
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    cli

    mov ax, 0x1000
    mov ss, ax
    mov sp, 0xFFFE

    mov ax, 0x1000
    mov ds, ax
    mov es, ax

[BITS 32]
    o32 lgdt [rel gdt_descriptor]

[BITS 16]
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump directly to pm_stub within 64KB range
    jmp 0x08:pm_stub

[BITS 32]
section .text
align 16
pm_stub:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, [stack_top_address]

    call early_kernel_main

.halt_loop:
    cli
    hlt
    jmp .halt_loop