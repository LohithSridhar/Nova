HOST?=i686-elf
SCREEN?=vga
KEYBOARD?=ps2
OSNAME?=Nova Electron
OSVERSION?=v0.1.9.0
ISONAME?=NovaElectron.iso
OSFULLNAME?=$(OSNAME) ($(OSVERSION))
BOOTSPLASH?=false
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

CFLAGS=-O2 -g -fstack-protector-strong
CPPFLAGS=
LDFLAGS=
LIBS=

# Configure the cross-compiler to use the desired system root.
SYSROOT=sysroot/
PLAIN_CC=$(CC) --sysroot=$(SYSROOT)

# Work around the fact that -elf gcc targets don't have a system include directory because they're configured with --without-headers rather than --with-sysroot.
CC:=$(shell if echo "$(HOST)" | grep -Eq -- '-elf($$|-)'; then echo "$(PLAIN_CC) -isystem=$(INCLUDEDIR)"; else echo "$(PLAIN_CC)"; fi)

# The only reason I'm keeping this seperate is because I have a feeling I'll need to change this for userspace.
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Werror -Wextra
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc
ARCHDIR=arch/$(ARCH)

include libc/make.config
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

KERNEL_C_OBJS=$(KERNEL_ARCH_OBJS) $(DRIVER_OBJS) kernel/kernel/kernel.o kernel/kernel/neutrino.o
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

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

LIBC_OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a

.PHONY: all headers install-kernel install-libs grub-cfg build run clean cycle line-count quick-run Nova.iso NovaOS.iso iso

all: nova.kernel $(BINARIES)

# I'm pretty sure that this will change as I add more projects but whatever
headers:
	@$(info Building system root directory...)
	@mkdir -p "$(SYSROOT)"
	@mkdir -p $(SYSROOT)$(INCLUDEDIR)
	@$(info Copying headers...)
	@echo '#define OS_NAME "$(OSNAME)"\n#define OS_VERSION "$(OSVERSION)"\n#define \
	OS_FULL_NAME "$(OSFULLNAME)"\n// osname.h - The only purpose of this header \
	is to centralise our OS name in here.' > libc/include/sys/osname.h
	@cp -R */include/. $(SYSROOT)$(INCLUDEDIR)/.

nova.kernel: $(KERNEL_OBJS) kernel/$(ARCHDIR)/link.ld $(BINARIES)
	@$(info Linking kernel files into $@...)
	@$(CC) -T $(LINK_FILE) -o $@ $(CFLAGS) $(LINK_LIST)
	$(GRUB)-file --is-x86-multiboot nova.kernel

libc.a: $(LIBC_OBJS)
	@$(info Compressing and Archiving libc.a...)
	@$(AR) rcs $@ $(LIBC_OBJS)

libk.a: $(LIBK_OBJS)
	$(info Compressing and Archiving libk.a...)
	@$(AR) rcs $@ $(LIBK_OBJS)

kernel/$(ARCHDIR)/crtbegin.o kernel/$(ARCHDIR)/crtend.o:
	$(info Fetching $@...)
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

%.o: %.c
	$(info gcc -std=gnu11 $<)
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	$(info as $<)
	@$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.libk.o: %.c
	$(info gcc -std=gnu11 $<)
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

%.libk.o: %.s
	$(info as $<)
	@$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

install-kernel: nova.kernel
	$(info Installing kernel...)
	@mkdir -p $(SYSROOT)$(BOOTDIR)
	@cp nova.kernel $(SYSROOT)$(BOOTDIR)

install-libs: $(BINARIES)
	$(info Installing libraries...)
	@mkdir $(SYSROOT)$(LIBDIR)
	@cp $(BINARIES) $(SYSROOT)$(LIBDIR)

build: headers install-libs install-kernel

grub-cfg:
	$(info Creating the grub configuration directory...)
	@mkdir -p isodir/boot/grub
	@cp $(SYSROOT)$(BOOTDIR)/nova.kernel isodir/boot/nova.kernel

	@if [ "$(BOOTSPLASH)" = "true" ]; then \
		echo "Bootsplash enabled."; \
		cp -r kernel/$(ARCHDIR)/grub isodir/boot/; \
		echo '\nmenuentry "$(OSFULLNAME)" {\nmultiboot /boot/nova.kernel\nboot\n}' >> isodir/boot/grub/grub.cfg; \
		cp kernel/kernel/bootsplash.png isodir/boot/grub/bootsplash.png; \
	else \
		echo "Bootsplash not enabled."; \
		echo 'set timeout=0\nset menu_hidden=true\nmenuentry "$(OSNAME)" {\nmultiboot /boot/nova.kernel\nboot\n}' > isodir/boot/grub/grub.cfg; \
	fi

$(ISONAME) NovaOS.iso Nova.iso iso: 
	@make build
	@make grub-cfg

	@$(GRUB)-mkrescue -o $(ISONAME) isodir
	$(info mkrescue -o $(ISONAME))

run: $(ISONAME)
	$(info Make sure to quit qemu using the close button and not CTRL+C to continue any processes.)
	qemu-system-$(ARCH) -k en-us -cdrom $(ISONAME)

quick-run:
	qemu-system-$(ARCH) -k en-us -cdrom $(ISONAME)

debug: $(ISONAME)
	(qemu-system-$(ARCH) -k en-us -s -S -cdrom $(ISONAME) &)
	(sleep 1 && gdb sysroot/boot/nova.kernel)
	make clean

clean:
	$(info Deleting major binaries...)
	@rm -rf nova.kernel $(BINARIES)
	$(info Deleting Objects and Dependancies...)
	@rm -rf $(KERNEL_OBJS) $(LIBC_OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o */*/*/*.o */*/*/*/*.o
	@rm -rf $(KERNEL_OBJS:.o=.d) $(LIBC_OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d */*/*/*.d */*/*/*/*.d
	$(info Deleting file structures...)
	@rm -rf sysroot isodir $(ISONAME)
	$(info Deleting generated headers...)
	@rm -rf libc/include/sys/osname.h

cycle: run clean

line-count:
	$(info Finding line count across .c, .h, .config, Makefile, .ld, .s, .config, and .cfg...")
	@find . -type f \( -name "*.ld" -o -name "Makefile" -o -name "*.s" -o -name "*.c" -o -name "*.h" -o -name \
	"make.config" -name "*.cfg" \) -print0 | xargs -0 wc -l | tail -n 1

-include $(KERNEL_OBJS:.o=.d)
-include $(LIBC_OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)