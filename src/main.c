#include "cpu/idt.h"
#include "eaos.h"
#include "hw/keyboard.h"
#include "hw/timer.h"
#include "io.h"
#include "irq.h"
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

void div_by_zero_test(void);
void alloc_test(void);
void do_absolutely_nothing(void);

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

    alloc_test();

    kbd_init();
    timer_init();

    init_pic();
    // div_by_zero_test();

    fb_printc(&term, '\n');
    kinfo(" *  Startup Complete!  * ");
    spin();
}

// Courtesy of Daniel Adibi (Adibi Productions® Coding™)
void do_absolutely_nothing(void) {
    // this actually hard resets the system,
    // probably because I don't have float priviliges yet.
    u64 lawsmat_sudios = 3; 
    u64 final_result = 2;
    if ((lawsmat_sudios-2)*(3*3^2-2)+(657898765432457%(2+2)) > 0){
        final_result += lawsmat_sudios / final_result;
        for (u32 i = 1; i <= 100000; i++) {
            for (u32 j = 2; j <= 100000; j++){
                volatile u8 adibus_number = 0 + 1;
                0;
                1;
            }
        }
    }
}

void div_by_zero_test(void) {
    kwarn("dividing by zero to ensure that the div/0 handler works!");

    volatile u8 a = 59;
    volatile u8 b = 0;
    volatile u8 _c = a / b;

    kwarn("resuming after div by zero");
}

#define MEMTEST_VALIDATION_NUMBER 0x42
#define MEMTEST_PAGES 3
#define XSTR(s) STR(s)
#define STR(s) #s
#define MEMTEST_PASS "Memtest passed! All numbers were " XSTR(MEMTEST_VALIDATION_NUMBER)
#define MEMTEST_FAIL "Memtest failed! Not all numbers were " XSTR(MEMTEST_VALIDATION_NUMBER) ". See output above."

void alloc_test(void) {
    kinfo("allocating memory!");
    void* page = kalloc(MEMTEST_PAGES);

    memset(page, MEMTEST_VALIDATION_NUMBER, MEM_PAGE_SIZE * MEMTEST_PAGES);

    isize i_bad_at = -1;
    for (usize i = 0; i < MEM_PAGE_SIZE * MEMTEST_PAGES; i++) {
        if ((*(u8*) (page + i)) != MEMTEST_VALIDATION_NUMBER) {
            i_bad_at = i;
            kwarn("not " XSTR(MEMTEST_VALIDATION_NUMBER) " at:");
            print_number((u64) (page + i));
        }
    }

    if (i_bad_at == -1) {
        struct log_level pass = {
            .name = "pass",
            .color = 0x7cff6e
        };
        kmsg(&pass, MEMTEST_PASS);
    } else {
        struct log_level fail = {
            .name = "fail",
            .color = 0xff6e6e
        };
        kmsg(&fail, MEMTEST_FAIL);
    }
}