# eaos
> an operating system

# running the OS
There are two primary ways to get the OS up and running:
## emulation
To automatically compile and run the bootloader and kernel in qemu,
you can use `make qemu`

## getting an `eaos.img` file (for real hardware)
To get an image file that you can burn to a flash drive,
you can simply use the `make img` target. It will produce an
`eaos.img` file in the `build` folder.