#include "hw/keyboard.h"
#include "io.h"
#include "irq.h"
#include "log.h"

INTR(kbd_handle);

void kbd_init(void) {
    kinfo("setting up keyboard...");
    
    irq_handle(0x01, kbd_handle_entry);
}

void kbd_handle(void) {
    kinfo("letter!");

    u8 cha = inb(0x60);
    print_number(cha);

    pic_eoi(0x01);
}

char kbd_getchar(void) {
    kwarn("todo: kbd_getchar()");
    return '?';
}