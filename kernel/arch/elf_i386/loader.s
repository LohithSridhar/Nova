; boot.asm -- 512-byte boot sector for BIOS x86 (i686-elf)
; NASM syntax

BITS 16
ORG 0x7c00

; --- Boot jump and OEM name ---
jmp short start                ; 2 bytes
nop                            ; 1 byte
db 'MSDOS5.0'                  ; OEM name (8 bytes, pad as needed)

; --- BPB (exact FAT32 layout, must match mkfs.fat and FAT spec) ---
dw 512                         ; Bytes per sector
db 8                           ; Sectors per cluster
dw 32                          ; Reserved sectors
db 2                           ; Number of FATs
dw 0                           ; Max root dir entries (0 for FAT32)
dw 0                           ; Total sectors (for FAT32 must be 0)
db 0xF8                        ; Media descriptor
dw 0                           ; Sectors per FAT (FAT32 must be 0)
dw 63                          ; Sectors per track
dw 255                         ; Number of heads
dd 0                           ; Hidden sectors
dd 65536                       ; Total sectors (use the right value for image size)
dd 512                         ; Sectors per FAT32
dw 0                           ; Extended flags
dw 0                           ; Filesystem version
dd 2                           ; Root cluster (usually 2)
dw 1                           ; FSInfo sector
dw 6                           ; Backup boot sector
resb 12                        ; Reserved for FAT32
db 0                           ; Drive number
db 0                           ; Reserved
db 0x29                        ; Boot signature
dd 0x12345678                  ; Volume serial number
db 'NOVAKERNEL '               ; Volume label (11 chars, pad with spaces)
db 'FAT32   '                  ; Filesystem type (8 chars, pad with spaces)

start:
    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7c00
    mov si, BOOT_DRIVE
    mov [si], dl

    ; Load kernel (sectors after bootsector) to address 0x1000:0x0000 (0x10000)
    mov bx, 0x1000     ; ES = 0x1000, BX = 0, so ES:BX = 0x10000
    mov es, bx
    xor bx, bx
    mov ah, 0x02       ; BIOS read sector function
    mov al, NUM_SECTORS ; Number of sectors to load (you must update this!)
    mov ch, 0x00       ; Cylinder
    mov dh, 0x00       ; Head 0
    mov cl, 0x02       ; Start at sector 2 (after bootsector)
    mov dl, [BOOT_DRIVE]
    int 0x13
    jc disk_error

    ; Enable A20 (optional: for >1MB access)
    call enable_a20

    ; Setup GDT
    lgdt [gdt_descriptor]

    ; Enter protected mode
	mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEL:protected_mode

disk_error:
    hlt

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

; === Begin protected mode stage ===

BITS 32
protected_mode:
    ; Set up segments: use the selectors from your custom GDT
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000      ; stack pointer (change as you wish)

    ; Jump to kernel entry point (0x085000, offset 0)
    jmp 0x085000

; --- GDT and GDTR selectors ---
ALIGN 8
gdt_start:
    dq 0
gdt_code: dq 0x00CF9A000000FFFF
gdt_data: dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEL   equ gdt_code - gdt_start
DATA_SEL   equ gdt_data - gdt_start

BOOT_DRIVE db 0
NUM_SECTORS equ 9         ; Update to fit kernel.bin size! (Also, JUST 9???)

times 510-($-$$) db 0
dw 0xAA55