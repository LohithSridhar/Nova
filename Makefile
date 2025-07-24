ARCHPARAMS = elf_i386
KERNEL_OBJECTS = kernel/arch/$(ARCHPARAMS)/loader.o kernel/kernel/kernel.o kernel/arch/$(ARCHPARAMS)/tty.o \
kernel/arch/$(ARCHPARAMS)/gdt.o kernel/arch/$(ARCHPARAMS)/gdt_flush.o
LIBC_OBJECTS = libc/stdio/printf.o libc/stdio/putchar.o libc/stdio/puts.o libc/stdlib/abort.o libc/string/memcmp.o \
libc/string/memcpy.o libc/string/memmove.o libc/string/memset.o libc/string/strlen.o
LIBK_OBJECTS = $(LIBC_OBJECTS:.o=.libk.o)
HEADERS = kernel/arch/$(ARCHPARAMS)/vga.h kernel/include/kernel/tty.h
CC = i686-elf-gcc --sysroot=iso -isystem=/usr/include/
CFLAGS = -m32 -O2 -g -ffreestanding -Wall -Wextra -Werror -c
LDFLAGS = -T kernel/arch/$(ARCHPARAMS)/link.ld -m$(ARCHPARAMS)
AS = nasm
ASFLAGS = -f elf
LD = i686-elf-ld
AR = i686-elf-ar
CRTI_OBJ=kernel/arch/elf_i386/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=kernel/arch/elf_i386/crtn.o
OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(KERNEL_OBJECTS) $(LIBK_OBJECTS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:=$(CRTI_OBJ) $(KERNEL_OBJECTS) $(LIBK_OBJECTS) $(CRTN_OBJ)

all: kernel.elf

headers:
	mkdir -p iso/usr/include/
	cp -r kernel/include/* iso/usr/include/
	cp -r kernel/arch/$(ARCHPARAMS)/*.h iso/usr/include
	cp -r libc/include/* iso/usr/include/

libk.a: headers $(LIBK_OBJECTS)
	$(AR) rcs $@ $(LIBK_OBJECTS)

envsetup: libk.a
	mkdir -p iso/boot/grub/
	mkdir iso/usr/lib/
	cp kernel/arch/$(ARCHPARAMS)/grub/* iso/boot/grub/
	cp libk.a iso/usr/lib/

kernel.elf: envsetup $(KERNEL_OBJECTS) $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)
	$(LD) $(LDFLAGS) $(OBJ_LINK_LIST) -o kernel.elf

os.iso: kernel.elf
	mv kernel.elf iso/boot/kernel.elf
	mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os \
	-input-charset utf8 -quiet -boot-info-table -o os.iso iso

run: os.iso
	qemu-system-i386 -boot d -cdrom os.iso -m 4 -monitor stdio

%.libk.o: %.c
	$(CC) $(CFLAGS) -D__is_libk $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.0: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o os.iso kernel.elf libk.a
	rm -rf $(INTERNAL_OBJS)
	rm -rf iso

cleanrun:
	make run
	make clean