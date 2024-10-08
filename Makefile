# eaos build system

version := 0.1.0
limine_version := 8.0.13

ext_dir := ext
build_dir := build
source_dir := src
out_file := kernel.bin
bootloader_out := boot.bin
img_file := eaos.img
iso_file := eaos.iso

bootloader := $(source_dir)/boot.S

# get a list of all C and assembly files
source_files := $(filter-out $(bootloader),$(shell find $(source_dir) -name '*.c' -or -name '*.S'))

# get a list of the objects it should generate
objects := $(source_files:%=$(build_dir)/%.o)
bootloader_objects := $(bootloader:%=$(build_dir)/%.o)

# these are the generated depfiles that clang makes
depfiles := $(objects:.o:.d)

include_dirs := $(shell find $(source_dir) -type d) $(ext_dir)/limine/limine-$(limine_version)
I_flags := $(addprefix -I,$(include_dirs))

toolchain_location = ../llvm-project/build/bin

# -- flags --
# c compiler, linker, and assembler to use
CC := clang
LD := ld.lld
ASM := nasm

cross_args := \
	-target x86_64-pc-none \
	-ffreestanding \
	-nostdlib \
	-fno-builtin \
	-static \
	-nodefaultlibs \

# c compiler flags
CFLAGS := \
	-std=c23 \
	-Wall \
	-g \
	-O0 \
	$(cross_args)

# preprocessor flags
CPPFLAGS := \
	$(I_flags) \
	-MMD

LDFLAGS := \
	-e _start \
	-T linker.ld

# -- targets --
all: $(build_dir) $(build_dir)/$(out_file)

# link step
$(build_dir)/$(out_file): $(objects)
	$(LD) $(LDFLAGS) $^ -o $@

$(build_dir)/%.c.o: %.c
# 		make sure this directory exists in the build folder
	mkdir -p $(dir $@)
# 		compile!
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(build_dir)/%.S.o: %.S
	mkdir -p $(dir $@)
	$(ASM) $< -Wall -MD -o $@ -f elf64

$(build_dir):
	mkdir $(build_dir)

$(build_dir)/$(bootloader_out): $(bootloader)
	$(ASM) $(bootloader) -o $@ -f bin

$(build_dir)/$(iso_file): $(build_dir) $(ext_dir)/limine/limine-$(limine_version)/bin  $(build_dir)/$(bootloader_out) $(build_dir)/$(out_file) release-info
	rm -rf $(build_dir)/iso
	mkdir -p $(build_dir)/iso
	cp $(build_dir)/$(bootloader_out) $(build_dir)/iso/$(bootloader_out)
	cp $(build_dir)/$(out_file) $(build_dir)/iso/$(out_file)
	cp $(build_dir)/release.txt $(build_dir)/iso/release.txt
	mkdir -p $(build_dir)/iso/boot
	cp $(ext_dir)/limine/limine-$(limine_version)/bin/limine-uefi-cd.bin $(build_dir)/iso/boot/
	cp $(ext_dir)/limine/limine-$(limine_version)/bin/limine-bios-cd.bin $(build_dir)/iso/boot/
	cp $(ext_dir)/limine/limine-$(limine_version)/bin/limine-bios.sys $(build_dir)/iso/boot/
	cp limine.conf $(build_dir)/iso/boot/
	mkdir -p $(build_dir)/iso/EFI/BOOT
	cp $(ext_dir)/limine/limine-$(limine_version)/bin/BOOTX64.EFI $(build_dir)/iso/EFI/BOOT/
# -b $(bootloader_out)
	xorriso \
		-as mkisofs \
		-o $(build_dir)/$(iso_file) \
		-b boot/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot-part --efi-boot-image --protective-msdos-label \
		--efi-boot boot/limine-uefi-cd.bin \
		-p LAWSON \
		-V EAOS_BOOT_DRIVE \
		$(build_dir)/iso

	$(ext_dir)/limine/limine-$(limine_version)/bin/limine bios-install $(build_dir)/$(iso_file)

$(ext_dir)/limine:
	# download limine
	mkdir -p $(ext_dir)/limine
	curl -L https://github.com/limine-bootloader/limine/releases/download/v$(limine_version)/limine-$(limine_version).tar.xz -o ext/limine/limine-$(limine_version).tar.xz 
	tar xf $(ext_dir)/limine/limine-$(limine_version).tar.xz -C $(ext_dir)/limine/

$(ext_dir)/limine/limine-$(limine_version)/bin: $(ext_dir)/limine
	# build limine!
	cd $(ext_dir)/limine/limine-$(limine_version); \
		./configure --enable-uefi-x86-64 --enable-uefi-cd \
			--enable-bios --enable-bios-cd; \
		make

clean:
	rm -rf $(build_dir)

clean-ext:
	rm -rf $(ext_dir)

# img: $(build_dir)/$(img_file)

iso: $(build_dir)/$(iso_file)

# qemu: img
# 	qemu-system-x86_64 -drive format=raw,file=$(build_dir)/$(img_file)

qemu: iso
	qemu-system-x86_64 -cdrom $(build_dir)/$(iso_file)

debug-qemu: iso
	qemu-system-x86_64 -cdrom $(build_dir)/$(iso_file) -gdb tcp::10002 -S

release-info:
	rm -f $(build_dir)/release.txt
	touch $(build_dir)/release.txt
	echo "EAOS $(version)\nBuilt $(shell TZ=UTC date)" > $(build_dir)/release.txt

.PHONY: all clean clean-ext qemu iso release-info debug-qemu

# include all the dependency data for all the files generated
# by the c preprocessor (w/ -MD and -MMD flags)
-include $(depfiles)
