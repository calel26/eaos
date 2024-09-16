# eaos build system

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

include_dirs := $(shell find $(source_dir) -type d)
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
	$(cross_args)

# preprocessor flags
CPPFLAGS := \
	$(I_flags) \
	-MMD

LDFLAGS := \
	-e _start

# -- targets --
all: $(build_dir) $(build_dir)/$(out_file)

# link step
$(build_dir)/$(out_file): $(objects)
	$(LD) $(LDFLAGS) $< -o $@ --oformat binary

$(build_dir)/%.c.o: %.c
# 		make sure this directory exists in the build folder
	mkdir -p $(dir $@)
# 		compile!
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(build_dir)/%.S.o: %.S
	mkdir -p $(dir $@)
	$(ASM) $< -MD -o $@ -f elf

$(build_dir):
	mkdir $(build_dir)

# $(build_dir)/$(img_file): $(build_dir) $(build_dir)/$(bootloader_out) $(build_dir)/$(out_file)
# # write img file full of zeros
# 	dd if=/dev/zero of=$(build_dir)/$(img_file) bs=512 count=8192
# # copy bootsector
# 	dd if=$(build_dir)/$(bootloader_out) of=$(build_dir)/$(img_file) conv=notrunc bs=512 count=1 seek=0
# # copy kernel
# 	dd if=$(build_dir)/$(out_file) of=$(build_dir)/$(img_file) conv=notrunc bs=512 count=7680 seek=1

$(build_dir)/$(bootloader_out): $(bootloader)
#	$(LD) $(LDFLAGS) $< -o $@ --oformat binary
	$(ASM) $(bootloader) -o $@ -f bin

$(build_dir)/$(iso_file): $(build_dir) $(build_dir)/$(bootloader_out) $(build_dir)/$(out_file)
	mkdir -p $(build_dir)/iso
	cp $(build_dir)/$(bootloader_out) $(build_dir)/iso/$(bootloader_out)
	cp $(build_dir)/$(out_file) $(build_dir)/iso/$(out_file)
	mkisofs \
		-b $(bootloader_out) \
		-o build/$(iso_file) \
		-no-emul-boot \
		-boot-load-size 4 \
		-p LAWSON \
		-V EAOS_BOOT_DRIVE \
		$(build_dir)/iso

clean: $(build_dir)
	rm -rf $(build_dir)

# img: $(build_dir)/$(img_file)

iso: $(build_dir)/$(iso_file)

# qemu: img
# 	qemu-system-x86_64 -drive format=raw,file=$(build_dir)/$(img_file)

qemu: iso
	qemu-system-x86_64 -cdrom $(build_dir)/$(iso_file)

.PHONY: all clean qemu iso

# include all the dependency data for all the files generated
# by the c preprocessor (w/ -MD and -MMD flags)
-include $(depfiles)