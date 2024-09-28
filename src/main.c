#include "eaos.h"
#include "limine.h"
#include "framebuffer.h"
#include "log.h"

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

    fb_print(&term, "Welcome to ");
    term.active_color = 0xc663ff;
    fb_print(&term, "eaos");
    term.active_color = 0xffffff;
    fb_print(&term, "!\n===================\n\n");

    log_setterm(&term);
    kinfo("good things are happening");
    kwarn("oh no!");
    kerr("terrible things are happening now...");

    kpanic("No tasks left to do");
}
