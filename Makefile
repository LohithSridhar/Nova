ARCHPARAMS = elf_i386
BOOT_OBJECT = kernel/arch/$(ARCHPARAMS)/loader.o
KERNEL_OBJECTS = kernel/kernel/kernel.o kernel/arch/$(ARCHPARAMS)/tty.o kernel/arch/$(ARCHPARAMS)/gdt.o kernel/arch/$(ARCHPARAMS)/gdt_flush.o
LIBC_OBJECTS = libc/stdio/printf.o libc/stdio/putchar.o libc/stdio/puts.o libc/stdlib/abort.o libc/string/memcmp.o \
libc/string/memcpy.o libc/string/memmove.o libc/string/memset.o libc/string/strlen.o
LIBK_OBJECTS = $(LIBC_OBJECTS:.o=.libk.o)
HEADERS = kernel/arch/$(ARCHPARAMS)/vga.h kernel/include/kernel/tty.h
CC = i686-elf-gcc --sysroot=iso -isystem=/usr/include/
CFLAGS = -m32 -O2 -g -ffreestanding -Wall -Wextra -Werror -c
LDFLAGS = -g -T kernel/arch/$(ARCHPARAMS)/link.ld -m$(ARCHPARAMS)
AS = nasm
ASFLAGS = -g -f elf
LD = i686-elf-ld
AR = i686-elf-ar
OBJCOPY = i686-elf-objcopy
CRTI_OBJ=kernel/arch/elf_i386/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=kernel/arch/elf_i386/crtn.o
OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(KERNEL_OBJECTS) $(LIBK_OBJECTS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:=$(BOOT_OBJECT) $(CRTI_OBJ) $(KERNEL_OBJECTS) $(LIBK_OBJECTS) $(CRTN_OBJ)

# Unused... for now.
#isoinfo -i NovaOS.img -l | grep KERNEL.ELF | awk '{
#  sectors = int( ($6 + 2047) / 2048 )
#  print "KERNEL_LBA_START=" $1
#  print "KERNEL_SECTORS=" sectors
#}'

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
	cp libk.a iso/usr/lib/

KERNEL.BIN: envsetup $(KERNEL_OBJECTS) $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)
	$(LD) $(LDFLAGS) $(OBJ_LINK_LIST) -o kernel.elf
	$(OBJCOPY) -O binary kernel.elf KERNEL.BIN

NovaOS.img: KERNEL.BIN $(BOOT_OBJECT)
	dd if=/dev/zero bs=1M count=64 of=NovaOS.img
	mkfs.vfat -F 32 NovaOS.img
	dd if=$(BOOT_OBJECT) of=NovaOS.img bs=512 count=1 conv=notrunc
	mcopy -i NovaOS.img KERNEL.BIN ::/
	dd if=/dev/zero bs=512 count=1 >> kernel.bin
# NovaOS.img's FAT32 state is temporary until we install userspace. The following is just for show.
	mv kernel/arch/$(ARCHPARAMS)/loader.o iso/boot/loader.bin
	mv KERNEL.BIN iso/boot/KERNEL.BIN

run: NovaOS.img
	qemu-system-i386 -hda NovaOS.img -m 4

%.libk.o: %.c
	$(CC) $(CFLAGS) -D__is_libk $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

kernel/arch/$(ARCHPARAMS)/loader.o: kernel/arch/$(ARCHPARAMS)/loader.s
	$(AS) -f bin $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o NovaOS.img kernel.elf libk.a
	rm -rf $(INTERNAL_OBJS)
	rm -rf iso

cleanrun:
	make run
	make clean