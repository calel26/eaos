#include "cpu/idt.h"
#include "eaos.h"
#include "limine.h"
#include "framebuffer.h"
#include "log.h"
#include "mem/phys.h"

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

void start(void) {
    // make_it_purple();
    struct eaos_terminal term = {
        .line = 0,
        .active_color = 0xffffff,
        .cursor_x = 0,
        .framebuffer = fb_get_framebuffer()
    };

    fb_print(&term, "Welcome to ");
    term.active_color = 0xc663ff;
    fb_print(&term, "eaos");
    term.active_color = 0xffffff;
    fb_print(&term, "!\n===================\n\n");

    log_setterm(&term);
    kinfo("starting up!");

    // init_pmm();
    // u64 *a = kalloc(1);
    // *a = 5;
    setup_idt();

    kerr("dividing by zero to ensure that the div/0 handler works!");

    volatile u8 a = 59;
    volatile u8 b = 0;
    volatile u8 c = a / b;

    kpanic("no tasks left to do");
}
