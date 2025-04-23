#include "kshell/main.h"
#include "eaos.h"
#include "event.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "log.h"
#include "proc.h"
#include "scheduler.h"

static void break_here() {};

// this is bodged because it should
// probably live somewhere else...
static char bodge_getchar(void) {
    // wake up when there's a new char

    struct ev_subscription s = {
        .type = ev_st_process,
        .event = ev_keyboard,
        .params = {
            .keyboard = {
                .keydown = true,
                .keyup = false,
            },
        },
        .trigger = {
            .proc = get_proc_id()
        },
    };

    subscribe(s);

    // sleep
    schedule();

    char c = kbd_getchar();

    return c;
}

void kshell_main(void) {
    char input_buffer[256] = {0};
    usize i = 0;
    kinfo("ish shell [v0.1]");
    struct eaos_terminal* term = log_getterm();

    while (true) {
        char c = bodge_getchar();
        fb_printc(term, c);
        input_buffer[i++] = c;
    } 

    // we're done
    schedule();
}