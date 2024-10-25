#include "cpu/idt.h"
#include "eaos.h"
#include "limine.h"
#include "framebuffer.h"
#include "log.h"
#include "mem/phys.h"
#include "mem/util.h"

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

void div_by_zero_test();
void alloc_test();

void start(void) {
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

    setup_idt();
    init_pmm();

    // div_by_zero_test();
    alloc_test();

    kpanic("no tasks left to do");
}

void div_by_zero_test() {
    kerr("dividing by zero to ensure that the div/0 handler works!");

    volatile u8 a = 59;
    volatile u8 b = 0;
    volatile u8 c = a / b;

    kwarn("resuming after div by zero");
}

#define MEMTEST_VALIDATION_NUMBER 0x42

void alloc_test() {
    kinfo("allocating memory!");
    void* page = kalloc(1);

    memset(page, MEMTEST_VALIDATION_NUMBER, MEM_PAGE_SIZE);

    isize i_bad_at = -1;
    for (usize i = 0; i < MEM_PAGE_SIZE; i++) {
        if ((*(u8*) (page + i)) != MEMTEST_VALIDATION_NUMBER) { i_bad_at = i; break; }
    }

    if (i_bad_at == -1) {
        kinfo("All 42!");
    } else {
        kwarn("It's not 42 at:");
        print_number((u64) i_bad_at);
    }
}