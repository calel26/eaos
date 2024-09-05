# eaos build system

build_dir := build
source_dir := src
out_file := kernel.bin

# get a list of all C or assembly (.S) files
source_files := $(shell find $(source_dir) -name '*.c' -or -name '*.S')

# get a list of the objects it should generate
objects := $(source_files:%=$(build_dir)/%.o)

# these are the generated depfiles that clang makes
depfiles := $(objects:.o:.d)

include_dirs := $(shell find $(source_dir) -type d)
I_flags := $(addprefix -I,$(include_dirs))

# -- flags --
# c compiler to use
CC := clang

cross_args := \
    -ffreestanding \
    -target x86_64-pc-none \

# c compiler flags
CFLAGS := \
	$(cross_args)

# preprocessor flags
CPPFLAGS := \
	$(I_flags) \
	-MMD

LDFLAGS := \
	-fuse-ld=lld \
	$(cross_args) \

# -- targets --
all: $(build_dir) $(build_dir)/$(out_file)

# link step
$(build_dir)/$(out_file): $(objects)
	$(CC) $(objects) -o $@ $(LDFLAGS)

$(build_dir)/%.c.o: %.c
# 		make sure this directory exists in the build folder
	mkdir -p $(dir $@)
# 		compile!
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(build_dir):
	mkdir $(build_dir)

clean:
	rm -rf $(build_dir)

.PHONY: all clean

# include all the dependency data for all the files generated
# by the c preprocessor (w/ -MD and -MMD flags)
-include $(depfiles)