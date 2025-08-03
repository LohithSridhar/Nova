HOST?=i686-elf
SCREEN?=vga
KEYBOARD?=ps2
OSNAME=NovaOS 0.1.7.0
ARCH:=$(shell if echo "$(HOST)" | grep -Eq 'i[[:digit:]]86-'; then echo i386; else echo "$(HOST)" | grep -Eo '^[[:alnum:]_]*'; fi)
SYSTEM_HEADER_PROJECTS=libc kernel
PROJECTS=libc kernel

MAKE=make
AR=$(HOST)-ar
AS=$(HOST)-as
CC=$(HOST)-gcc
GRUB=$(HOST)-grub

PREFIX=/usr
EXEC_PREFIX=$(PREFIX)
BOOTDIR=/boot
LIBDIR=$(EXEC_PREFIX)/lib
INCLUDEDIR=$(PREFIX)/include

CFLAGS=-O2 -g
CPPFLAGS=
LDFLAGS=
LIBS=

# Configure the cross-compiler to use the desired system root.
SYSROOT=$(shell pwd)/sysroot
PLAIN_CC=$(CC) --sysroot=$(SYSROOT)

# Work around the fact that -elf gcc targets don't have a system include directory because they're configured with --without-headers rather than --with-sysroot.
CC:=$(shell if echo "$(HOST)" | grep -Eq -- '-elf($$|-)'; then echo "$(PLAIN_CC) -isystem=$(INCLUDEDIR)"; else echo "$(PLAIN_CC)"; fi)

# The only reason I'm keeping this seperate is because I have a feeling I'll need to change this for userspace.
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Werror -Wextra
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc
ARCHDIR=arch/$(ARCH)

include kernel/$(ARCHDIR)/make.config
include libc/$(ARCHDIR)/make.config
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
KERNEL_CPPFLAGS:=$(CPPFLAGS)  -D__is_kernel -Iinclude $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)

LIBC_CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) -D__is_libk $(KERNEL_ARCH_CPPFLAGS)
KERNEL_ARCH_OBJS:=$(strip $(KERNEL_ARCH_OBJS))
ARCH_FREEOBJS:=$(strip $(ARCH_FREEOBJS))

# NOTE: the video driver has the same $(SCREEN) name as the tty driver.
DRIVER_OBJS=kernel/$(ARCHDIR)/drivers/tty/$(SCREEN)_tty.o kernel/$(ARCHDIR)/drivers/keyboard/$(KEYBOARD)_keyboard.o

KERNEL_C_OBJS=$(KERNEL_ARCH_OBJS) $(DRIVER_OBJS) kernel/kernel/kernel.o
KERNEL_C_OBJS:=$(strip $(KERNEL_C_OBJS))

KERNEL_OBJS=kernel/$(ARCHDIR)/crti.o kernel/$(ARCHDIR)/crtbegin.o $(KERNEL_C_OBJS) kernel/$(ARCHDIR)/crtend.o kernel/$(ARCHDIR)/crtn.o

LINK_FILE=kernel/$(ARCHDIR)/link.ld

LINK_LIST=\
$(LDFLAGS) \
kernel/$(ARCHDIR)/crti.o \
kernel/$(ARCHDIR)/crtbegin.o \
$(KERNEL_C_OBJS) \
$(LIBS) \
kernel/$(ARCHDIR)/crtend.o \
kernel/$(ARCHDIR)/crtn.o \

FREEOBJS=\
$(ARCH_FREEOBJS) \
libc/stdio/printf.o \
libc/stdio/putchar.o \
libc/stdio/puts.o \
libc/stdio/getchar.o \
libc/stdio/fgets.o \
libc/stdio/scanf.o \
libc/stdlib/abort.o \
libc/string/memcmp.o \
libc/string/memcpy.o \
libc/string/memmove.o \
libc/string/memset.o \
libc/string/strlen.o \

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

LIBC_OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a

.PHONY: all headers install-kernel install-libs grub-cfg build run clean

all: nova.kernel $(BINARIES)

headers:
	mkdir -p "$(SYSROOT)"
# I'm pretty sure that this will change as I add more projects but whatever
	mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -R */include/. $(SYSROOT)$(INCLUDEDIR)/.

nova.kernel: $(KERNEL_OBJS) kernel/$(ARCHDIR)/link.ld $(BINARIES)
	$(CC) -T $(LINK_FILE) -o $@ $(CFLAGS) $(LINK_LIST)
	$(GRUB)-file --is-x86-multiboot nova.kernel

libc.a: $(LIBC_OBJS)
	$(AR) rcs $@ $(LIBC_OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

kernel/$(ARCHDIR)/crtbegin.o kernel/$(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

%.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.libk.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

%.libk.o: %.s
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

install-kernel: nova.kernel
	mkdir -p $(SYSROOT)$(BOOTDIR)
	cp nova.kernel $(SYSROOT)$(BOOTDIR)

install-libs: $(BINARIES)
	mkdir $(SYSROOT)$(LIBDIR)
	cp $(BINARIES) $(SYSROOT)$(LIBDIR)

build: headers
	make install-libs
	make install-kernel

NovaOS.iso: build
	mkdir -p isodir/boot/grub
	cp $(SYSROOT)$(BOOTDIR)/nova.kernel isodir/boot/nova.kernel
	printf 'menuentry "$(OSNAME)" {\nmultiboot /boot/nova.kernel\n}\n' > isodir/boot/grub/grub.cfg
	$(GRUB)-mkrescue -o $@ isodir

run: NovaOS.iso
	echo "Make sure to quit qemu using the close button and not CTRL+C to continue any processes."
	qemu-system-$(ARCH) -k en-us -cdrom NovaOS.iso

quickrun:
	qemu-system-$(ARCH) -k en-us -cdrom NovaOS.iso

debug: NovaOS.iso
	(qemu-system-$(ARCH) -k en-us -s -S -cdrom NovaOS.iso &)
	(sleep 1 && gdb sysroot/boot/nova.kernel)
	make clean

clean:
	rm -rf nova.kernel $(BINARIES)
	rm -rf $(KERNEL_OBJS) $(LIBC_OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o */*/*/*.o */*/*/*/*.o
	rm -rf $(KERNEL_OBJS:.o=.d) $(LIBC_OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d */*/*/*.d */*/*/*/*.d
	rm -rf sysroot isodir NovaOS.iso

cycle:
	make run
	make clean

-include $(KERNEL_OBJS:.o=.d)
-include $(LIBC_OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)