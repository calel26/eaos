#include "hw/timer.h"
#include "framebuffer.h"
#include "irq.h"
#include "log.h"

INTR(timer_handle)

void timer_init(void) {
    kinfo("setting up PIC timer!");

    irq_handle(0x00, timer_handle_entry);
}

void timer_handle(void) {
    fb_printc(log_getterm(), '*');
    pic_eoi(0x00);
}