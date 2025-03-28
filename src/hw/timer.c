#include "hw/timer.h"
#include "event.h"
#include "framebuffer.h"
#include "irq.h"
#include "log.h"

INTR(timer_handle)

struct timer_demo {
    bool enabled;
    usize tick;
    usize line, x;
};

static struct timer_demo demo = {
    .tick = 0,
    .enabled = false
};

void timer_init(void) {
    kinfo("setting up PIC timer!");

    irq_handle(0x00, timer_handle_entry);
    // demo.enabled = true;
}

static bool timer_predicate(union ev_subscription_params *p) {
    // FIXME: ignores config option `milliseconds`
    return true;
}

void timer_handle(void) {
    // send to the event bus!
    handle(ev_timer, timer_predicate);

    if (!demo.enabled) goto done;

    // step the demo
    struct eaos_terminal *term = log_getterm();
    if (demo.tick == 0) {
        fb_print(term, "Timer Demo: ");
        demo.line = term->line;
        demo.x = term->cursor_x;
    }

    char stars[] = "*****************";

    usize old_line = term->line;
    usize old_x = term->cursor_x;
    usize old_color = term->active_color;
    term->line = demo.line;
    term->cursor_x = demo.x;

    for (usize i = 0; i < sizeof(stars) - 1; i++) {
        if (demo.tick % sizeof(stars) - 1 == i) {
            term->active_color = 0x03a9fc;
        } else {
            term->active_color = 0x7d7d7d;
        }
        fb_printc(term, stars[i]);
    }

    term->line = old_line;
    term->cursor_x = old_x;
    term->active_color = old_color;

    if (demo.tick == 0) {
        fb_printc(term, '\n');
    }

    ++ demo.tick;

done:
    pic_eoi(0x00);
}