#include "eaos.h"
#include "limine.h"
#include "framebuffer.h"

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

void start(void) {
    bool a = false;
    i32 b = 5;

    // make_it_purple();
    struct eaos_terminal term = {
        .line = 0,
        .active_color = 0xffffff,
        .cursor_x = 0,
        .framebuffer = fb_get_framebuffer()
    };

    fb_print(&term, "hola mundo\n- eaos\n");
    fb_print(&term, "===========\n\n");
    term.active_color = 0x7d1dcc;
    fb_print(&term, "I love being purple\n");
    fb_print(&term, "- patrick");

    // loop forever
    while (1) {
        __asm__( "hlt" );
    }
}
