# eaos
> an operating system

# dependencies
- clang
- nasm
- cdrtools (only required if you want to make an iso file)

# running the OS
There are two primary ways to get the OS up and running:
## emulation
To automatically compile and run the bootloader and kernel in qemu,
you can use `make qemu`

## getting an `eaos.iso` file (for real hardware)
To get an image file that you can burn to a flash drive,
you can simply use the `make iso` target. It will produce an
`eaos.iso` file in the `build` folder.