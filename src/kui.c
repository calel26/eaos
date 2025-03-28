#include "kui.h"
#include "event.h"
#include "log.h"
#include "phys.h"
#include "proc.h"
#include "scheduler.h"
#include "ui/button.h"
#include "ui/element.h"
#include "ui/ui.h"

static bool signal_start_init = false;

void placeholder_init(void) {
    kinfo("INITTING!");
    struct ev_subscription s = {
        .event = ev_keyboard,
        .params = {
            .keyboard = {
                .keyup = false,
                .keydown = true
            }
        },
        .type = ev_st_process,
        .trigger = {
            .proc = 1
        },
    };

    subscribe(s);

    schedule();

    kinfo("HELLO I AM THE INIT FUNCTION AND I RAN SUCCESSFULLY!!!!!!");

    // yields forever, because all the subscriptions are gone.
    schedule();
}

static void on_click(struct ui_state *state) {
    kinfo("clicked! starting /sbin/init...");
    struct ui_button_config *bconf = state->elem->conf;
    bconf->text = "clicked!";
    bconf->disabled = true;

    usize procn = mkproc("init", "/sbin/init");
    kinfo(ksprintf("created init process as PID %d", procn));
    if (procn != 1) {
        kpanic("init not started as PID 1");
    }

    signal_start_init = true;
}

static struct ui_button_config conf = {
    .bbox = {
        .x = 400,
        .y = 400,
        .h = 100,
        .w = 200,
    },
    .color = 0xFF0000,
    .hover_color = 0x00FF00,
    .on_click = on_click,
    .text = "run init process!"
};

void start_kui() {
    kinfo("starting kernel UI");

    struct ui_elem* init = kalloc(1);
    *init = ui_mk_button(&conf);
    ui_add_elem(init);
    
    redraw_all();
}

bool should_start_init() {
    return signal_start_init;
}