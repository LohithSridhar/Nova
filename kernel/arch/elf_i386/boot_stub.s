[BITS 16]
org 0x7C00

start:
    cli                     ; Disable interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFF0

    ; Load kernel to 0x100000 (segment:offset 0x1000:0x0000)
    mov si, 0              ; Sector offset after boot sector (LBA = 1)
    mov bx, 0x0000
    mov es, 0x1000         ; es:bx = 0x100000
    mov dh, 0              ; Head 0
    mov cl, 2              ; Start at sector 2 (sector 1 has bootloader)
    mov ch, 0              ; Cylinder 0

    mov di, KERNEL_SECTORS ; Sectors to load
.load_kernel:
    mov ah, 2              ; INT 13h/read sectors
    mov al, 1              ; Read 1 sector at a time
    mov dl, 0x80           ; First hard disk
    push cx
    int 0x13
    jc .fail
    add bx, 512
    inc cl
    dec di
    pop cx
    jnz .load_kernel

    ; Set up GDT for protected mode (flat 4GB code/data)
    cli
    lgdt [gdt_descriptor]

    ; Enable A20 line
    in al, 0x92
    or al, 0x02
    out 0x92, al

    ; Set PE bit
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to 32-bit code: 0x08:0x00000000
    jmp 0x08:0

[BITS 32]
[org 0x100000]             ; This matches where we loaded the kernel

protected_mode:
    ; Setup data segments for flat model
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Now jump to kernel entry (assumed offset 0 in loaded image)
    ; Replace 0 for a different entry offset if needed
    jmp kernel_entry

.fail:
    hlt
    jmp .fail

KERNEL_SECTORS  equ 8      ; Adjust to kernel size in 512-byte sectors

align 8
gdt_start:
    dq 0x0000000000000000          ; Null descriptor
    dq 0x00CF9A000000FFFF          ; Code, base 0, limit 4GB, RX
    dq 0x00CF92000000FFFF          ; Data, base 0, limit 4GB, RW
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0     ; Pad to 510 bytes
dw 0xAA55                 ; Bootable signature
