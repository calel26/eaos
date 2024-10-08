# eaos
> an operating system

# dependencies
- clang (with c23 support)
- nasm
- xorriso
- mtools

# running the OS
There are two primary ways to get the OS up and running:

## emulation
To automatically compile and run the bootloader and kernel in qemu,
you can use `make qemu`

## getting an `eaos.iso` file
To get an image file that you can burn to a flash drive,
you can simply use the `make iso` target. It will produce an
`eaos.iso` file in the `build` folder.

## running tools
To run tools from the `tool` folder, you can use the utility `tool.sh` script.
```sh
./tool.sh [tool]
```
