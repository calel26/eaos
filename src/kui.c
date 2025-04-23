#include "kui.h"
#include "eaos.h"
#include "event.h"
#include "framebuffer.h"
#include "kshell/main.h"
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

    schedule(); // won't yield until the subscription is awoken

    kinfo("HELLO I AM THE INIT FUNCTION AND I RAN SUCCESSFULLY!!!!!!");
    // drop into a shell

    kshell_main();
}

static void on_click(struct ui_state *state) {
    kinfo("clicked! starting init...");
    struct ui_button_config *bconf = state->elem->conf;
    bconf->text = "clicked!";
    bconf->disabled = true;

    signal_start_init = true;
}

static void explode_now(struct ui_state *state) {
    kinfo("self destruct activated.");
    struct ui_button_config *bconf = state->elem->conf;
    bconf->text = ":)";
    bconf->disabled = true;

    struct eaos_terminal *term = log_getterm();
    // todo: explode
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

static struct ui_button_config explode = {
    .bbox = {
        .x = 500,
        .y = 100,
        .h = 75,
        .w = 150
    },
    .color = 0xFF3300,
    .hover_color = 0xFF0000,
    .on_click = explode_now,
    .text = "explode"
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