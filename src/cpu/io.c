#include "io.h"
#include "eaos.h"

inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile (
        "inb %w1, %b0"
        : "=a"(ret)
        : "Nd"(port)
        : "memory"
    );
    return ret;
};

inline void outb(u16 port, u8 byte) {
    __asm__ volatile (
        "outb %b0, %w1"
        : : "a"(byte), "Nd"(port)
        : "memory"
    );
};

inline void io_wait(void) {
    outb(0x80, 0);
}